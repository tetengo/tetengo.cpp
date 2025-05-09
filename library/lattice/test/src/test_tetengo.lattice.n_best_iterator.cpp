/*! \file
    \brief An N-best lattice path iterator.

    Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <any>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/stl_interfaces/iterator_interface.hpp> // IWYU pragma: keep
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/connection.h>
#include <tetengo/lattice/constraint.h>
#include <tetengo/lattice/constraint.hpp>
#include <tetengo/lattice/constraintElement.h>
#include <tetengo/lattice/constraint_element.hpp>
#include <tetengo/lattice/entry.h>
#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/input.h>
#include <tetengo/lattice/input.hpp>
#include <tetengo/lattice/lattice.h>
#include <tetengo/lattice/lattice.hpp>
#include <tetengo/lattice/nBestIterator.h>
#include <tetengo/lattice/n_best_iterator.hpp>
#include <tetengo/lattice/node.h>
#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/node_constraint_element.hpp>
#include <tetengo/lattice/path.h>
#include <tetengo/lattice/path.hpp>
#include <tetengo/lattice/string_input.hpp>
#include <tetengo/lattice/unordered_map_vocabulary.hpp>
#include <tetengo/lattice/vocabulary.h>
#include <tetengo/lattice/vocabulary.hpp>
#include <tetengo/lattice/wildcard_constraint_element.hpp>


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

    int preceding_edge_cost(const tetengo::lattice::path& path_, const std::size_t node_index)
    {
        const auto& nodes = path_.nodes();
        assert(!std::empty(nodes));
        assert(0 < node_index && node_index < std::size(nodes));
        return nodes[node_index].preceding_edge_costs()[nodes[node_index - 1].index_in_step()];
    }

    int recalc_path_cost(const tetengo::lattice::path& path_)
    {
        const auto& nodes = path_.nodes();
        assert(!std::empty(nodes));
        auto cost = nodes[0].node_cost();
        for (std::size_t i = 1; i < std::size(nodes); ++i)
        {
            cost += preceding_edge_cost(path_, i);
            cost += nodes[i].node_cost();
        }
        return cost;
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

    int c_preceding_edge_cost(const std::vector<tetengo_lattice_node_t>& nodes, const std::size_t node_index)
    {
        assert(!std::empty(nodes));
        assert(0 < node_index && node_index < std::size(nodes));
        const auto preceding_index_in_step = nodes[node_index - 1].index_in_step;
        assert(preceding_index_in_step < nodes[node_index].preceding_edge_cost_count);
        return nodes[node_index].p_preceding_edge_costs[preceding_index_in_step];
    }

    int recalc_c_path_cost(const std::vector<tetengo_lattice_node_t>& nodes)
    {
        assert(!std::empty(nodes));
        auto cost = nodes[0].node_cost;
        for (std::size_t i = 1; i < std::size(nodes); ++i)
        {
            cost += c_preceding_edge_cost(nodes, i);
            cost += nodes[i].node_cost;
        }
        return cost;
    }

}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(lattice)
BOOST_AUTO_TEST_SUITE(cap)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    const std::vector<int>              preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
    auto                                node = tetengo::lattice::node::eos(1, &preceding_edge_costs, 5, 42);
    std::vector<tetengo::lattice::node> nodes{ std::move(node) };
    const tetengo::lattice::cap         cap_{ std::move(nodes), 24, 42 };
}

BOOST_AUTO_TEST_CASE(operator_less)
{
    BOOST_TEST_PASSPOINT();

    const std::vector<int>              preceding_edge_costs1{ 3, 1, 4, 1, 5, 9, 2, 6 };
    auto                                node1 = tetengo::lattice::node::eos(1, &preceding_edge_costs1, 5, 42);
    std::vector<tetengo::lattice::node> nodes1{ std::move(node1) };
    const tetengo::lattice::cap         cap1{ std::move(nodes1), 24, 42 };

    const std::vector<int>              preceding_edge_costs2{ 3, 1, 4, 1, 5, 9, 2, 6 };
    auto                                node2 = tetengo::lattice::node::eos(1, &preceding_edge_costs2, 5, 42);
    std::vector<tetengo::lattice::node> nodes2{ std::move(node2) };
    const tetengo::lattice::cap         cap2{ std::move(nodes2), 24, 42 };

    const std::vector<int>              preceding_edge_costs3{ 2, 7, 1, 8, 2, 8 };
    auto                                node3 = tetengo::lattice::node::eos(2, &preceding_edge_costs3, 3, 31);
    std::vector<tetengo::lattice::node> nodes3{ std::move(node3) };
    const tetengo::lattice::cap         cap3{ std::move(nodes3), 12, 4242 };

    BOOST_CHECK(!(cap1 < cap2));
    BOOST_CHECK(cap1 < cap3);
}

BOOST_AUTO_TEST_CASE(tail_path)
{
    BOOST_TEST_PASSPOINT();

    const std::vector<int>              preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
    auto                                node = tetengo::lattice::node::eos(1, &preceding_edge_costs, 5, 42);
    std::vector<tetengo::lattice::node> nodes{ std::move(node) };
    const tetengo::lattice::cap         cap_{ std::move(nodes), 24, 42 };

    BOOST_TEST(std::size(cap_.tail_path()) == 1U);
    BOOST_TEST(&cap_.tail_path()[0].preceding_edge_costs() == &preceding_edge_costs);
}

BOOST_AUTO_TEST_CASE(tail_path_cost)
{
    BOOST_TEST_PASSPOINT();

    const std::vector<int>              preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
    auto                                node = tetengo::lattice::node::eos(1, &preceding_edge_costs, 5, 42);
    std::vector<tetengo::lattice::node> nodes{ std::move(node) };
    const tetengo::lattice::cap         cap_{ std::move(nodes), 24, 42 };

    BOOST_TEST(cap_.tail_path_cost() == 24);
}

BOOST_AUTO_TEST_CASE(whole_path_cost)
{
    BOOST_TEST_PASSPOINT();

    const std::vector<int>              preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
    auto                                node = tetengo::lattice::node::eos(1, &preceding_edge_costs, 5, 42);
    std::vector<tetengo::lattice::node> nodes{ std::move(node) };
    const tetengo::lattice::cap         cap_{ std::move(nodes), 24, 42 };

    BOOST_TEST(cap_.whole_path_cost() == 42);
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE(n_best_iterator)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::n_best_iterator iterator{};
    }
    {
        const auto                p_vocabulary = create_cpp_vocabulary();
        tetengo::lattice::lattice lattice_{ *p_vocabulary };
        lattice_.push_back(to_input("[HakataTosu]"));
        lattice_.push_back(to_input("[TosuOmuta]"));
        lattice_.push_back(to_input("[OmutaKumamoto]"));

        auto                                    eos_node_and_preceding_edge_costs = lattice_.settle();
        const tetengo::lattice::n_best_iterator iterator{ lattice_,
                                                          std::move(eos_node_and_preceding_edge_costs.first),
                                                          std::make_unique<tetengo::lattice::constraint>() };
    }
    {
        const auto                p_vocabulary = create_cpp_vocabulary();
        tetengo::lattice::lattice lattice_{ *p_vocabulary };
        lattice_.push_back(to_input("[HakataTosu]"));
        lattice_.push_back(to_input("[TosuOmuta]"));
        lattice_.push_back(to_input("[OmutaKumamoto]"));

        auto eos_node_and_preceding_edge_costs = lattice_.settle();
        BOOST_CHECK_THROW(
            const tetengo::lattice::n_best_iterator iterator(
                lattice_,
                std::move(eos_node_and_preceding_edge_costs.first),
                std::unique_ptr<tetengo::lattice::constraint>()),
            std::invalid_argument);
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

        const auto             preceding_edge_cost_count = tetengo_lattice_lattice_settle(p_lattice, nullptr, nullptr);
        tetengo_lattice_node_t eos_node{};
        std::vector<int>       preceding_edge_costs(preceding_edge_cost_count, 0);
        tetengo_lattice_lattice_settle(p_lattice, &eos_node, std::data(preceding_edge_costs));
        const auto* const p_iterator =
            tetengo_lattice_nBestIterator_create(p_lattice, &eos_node, tetengo_lattice_constraint_createEmpty());
        BOOST_SCOPE_EXIT(p_iterator)
        {
            tetengo_lattice_nBestIterator_destroy(p_iterator);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_iterator);
    }
    {
        tetengo_lattice_node_t eos_node{};
        const auto* const      p_iterator =
            tetengo_lattice_nBestIterator_create(nullptr, &eos_node, tetengo_lattice_constraint_createEmpty());
        BOOST_TEST(!p_iterator);
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

        const auto* const p_iterator =
            tetengo_lattice_nBestIterator_create(p_lattice, nullptr, tetengo_lattice_constraint_createEmpty());
        BOOST_TEST(!p_iterator);
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

        const auto             preceding_edge_cost_count = tetengo_lattice_lattice_settle(p_lattice, nullptr, nullptr);
        tetengo_lattice_node_t eos_node{};
        std::vector<int>       preceding_edge_costs(preceding_edge_cost_count, 0);
        tetengo_lattice_lattice_settle(p_lattice, &eos_node, std::data(preceding_edge_costs));
        const auto* const p_iterator = tetengo_lattice_nBestIterator_create(p_lattice, &eos_node, nullptr);
        BOOST_TEST(!p_iterator);
    }
}

BOOST_AUTO_TEST_CASE(operator_dereference)
{
    BOOST_TEST_PASSPOINT();

    {
        const auto                p_vocabulary = create_cpp_vocabulary();
        tetengo::lattice::lattice lattice_{ *p_vocabulary };
        lattice_.push_back(to_input("[HakataTosu]"));
        lattice_.push_back(to_input("[TosuOmuta]"));
        lattice_.push_back(to_input("[OmutaKumamoto]"));

        auto                                    eos_node_and_preceding_edge_costs = lattice_.settle();
        const tetengo::lattice::n_best_iterator iterator{ lattice_,
                                                          std::move(eos_node_and_preceding_edge_costs.first),
                                                          std::make_unique<tetengo::lattice::constraint>() };

        const auto& path = *iterator;
        BOOST_TEST_REQUIRE(std::size(path.nodes()) == 3U);
        BOOST_TEST(!path.nodes()[0].value().has_value());
        BOOST_TEST(std::any_cast<std::string>(path.nodes()[1].value()) == "tsubame");
        BOOST_TEST(!path.nodes()[2].value().has_value());
    }
    {
        const tetengo::lattice::n_best_iterator iterator{};

        BOOST_CHECK_THROW([[maybe_unused]] const auto& dereferenced = *iterator, std::logic_error);
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

        const auto             preceding_edge_cost_count = tetengo_lattice_lattice_settle(p_lattice, nullptr, nullptr);
        tetengo_lattice_node_t eos_node{};
        std::vector<int>       preceding_edge_costs(preceding_edge_cost_count, 0);
        tetengo_lattice_lattice_settle(p_lattice, &eos_node, std::data(preceding_edge_costs));
        const auto* const p_iterator =
            tetengo_lattice_nBestIterator_create(p_lattice, &eos_node, tetengo_lattice_constraint_createEmpty());
        BOOST_SCOPE_EXIT(p_iterator)
        {
            tetengo_lattice_nBestIterator_destroy(p_iterator);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_iterator);

        const auto* const p_path = tetengo_lattice_nBestIterator_createPath(p_iterator);
        BOOST_SCOPE_EXIT(p_path)
        {
            tetengo_lattice_path_destroy(p_path);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_path);

        const auto node_count = tetengo_lattice_path_pNodes(p_path, nullptr);
        BOOST_TEST_REQUIRE(node_count == 3U);

        std::vector<tetengo_lattice_node_t> nodes{ node_count };
        const auto                          node_count_again = tetengo_lattice_path_pNodes(p_path, std::data(nodes));
        BOOST_TEST(node_count_again == 3U);
        BOOST_TEST(!tetengo_lattice_entryView_valueOf(nodes[0].value_handle));
        BOOST_TEST(
            *reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(nodes[1].value_handle)) ==
            "tsubame");
        BOOST_TEST(!tetengo_lattice_entryView_valueOf(nodes[2].value_handle));
    }
    {
        const auto* const p_path = tetengo_lattice_nBestIterator_createPath(nullptr);
        BOOST_TEST(!p_path);
    }
}

BOOST_AUTO_TEST_CASE(operator_equal)
{
    BOOST_TEST_PASSPOINT();

    {
        const auto                p_vocabulary = create_cpp_vocabulary();
        tetengo::lattice::lattice lattice_{ *p_vocabulary };
        lattice_.push_back(to_input("[HakataTosu]"));
        lattice_.push_back(to_input("[TosuOmuta]"));
        lattice_.push_back(to_input("[OmutaKumamoto]"));

        auto                                    eos_node_and_preceding_edge_costs1 = lattice_.settle();
        const tetengo::lattice::n_best_iterator iterator1{ lattice_,
                                                           std::move(eos_node_and_preceding_edge_costs1.first),
                                                           std::make_unique<tetengo::lattice::constraint>() };
        auto                                    eos_node_and_preceding_edge_costs2 = lattice_.settle();
        const tetengo::lattice::n_best_iterator iterator2{ lattice_,
                                                           std::move(eos_node_and_preceding_edge_costs2.first),
                                                           std::make_unique<tetengo::lattice::constraint>() };
        auto                                    eos_node_and_preceding_edge_costs3 = lattice_.settle();
        tetengo::lattice::n_best_iterator       iterator3{ lattice_,
                                                     std::move(eos_node_and_preceding_edge_costs3.first),
                                                     std::make_unique<tetengo::lattice::constraint>() };
        ++iterator3;
        const tetengo::lattice::n_best_iterator iterator_last{};

        BOOST_CHECK(iterator1 == iterator1);
        BOOST_CHECK(iterator1 == iterator2);
        BOOST_CHECK(iterator1 != iterator3);
        BOOST_CHECK(iterator1 != iterator_last);

        ++iterator3;
        ++iterator3;
        ++iterator3;
        ++iterator3;
        ++iterator3;
        ++iterator3;
        ++iterator3;
        ++iterator3;

        BOOST_CHECK(iterator3 == iterator_last);
    }
    {
        const auto                p_vocabulary = create_cpp_vocabulary();
        tetengo::lattice::lattice lattice_{ *p_vocabulary };
        lattice_.push_back(to_input("[HakataTosu]"));
        lattice_.push_back(to_input("[TosuOmuta]"));

        auto                                    eos_node_and_preceding_edge_costs1 = lattice_.settle();
        const tetengo::lattice::n_best_iterator iterator1{ lattice_,
                                                           std::move(eos_node_and_preceding_edge_costs1.first),
                                                           std::make_unique<tetengo::lattice::constraint>() };

        lattice_.push_back(to_input("[OmutaKumamoto]"));

        auto                                    eos_node_and_preceding_edge_costs2 = lattice_.settle();
        const tetengo::lattice::n_best_iterator iterator2{ lattice_,
                                                           std::move(eos_node_and_preceding_edge_costs2.first),
                                                           std::make_unique<tetengo::lattice::constraint>() };

        BOOST_CHECK(iterator1 != iterator2);
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

        const auto             preceding_edge_cost_count = tetengo_lattice_lattice_settle(p_lattice, nullptr, nullptr);
        tetengo_lattice_node_t eos_node{};
        std::vector<int>       preceding_edge_costs(preceding_edge_cost_count, 0);
        tetengo_lattice_lattice_settle(p_lattice, &eos_node, std::data(preceding_edge_costs));
        auto* const p_iterator =
            tetengo_lattice_nBestIterator_create(p_lattice, &eos_node, tetengo_lattice_constraint_createEmpty());
        BOOST_SCOPE_EXIT(p_iterator)
        {
            tetengo_lattice_nBestIterator_destroy(p_iterator);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_iterator);

        BOOST_TEST(tetengo_lattice_nBestIterator_hasNext(p_iterator));

        tetengo_lattice_nBestIterator_next(p_iterator);
        BOOST_TEST(tetengo_lattice_nBestIterator_hasNext(p_iterator));

        tetengo_lattice_nBestIterator_next(p_iterator);
        BOOST_TEST(tetengo_lattice_nBestIterator_hasNext(p_iterator));

        tetengo_lattice_nBestIterator_next(p_iterator);
        BOOST_TEST(tetengo_lattice_nBestIterator_hasNext(p_iterator));

        tetengo_lattice_nBestIterator_next(p_iterator);
        BOOST_TEST(tetengo_lattice_nBestIterator_hasNext(p_iterator));

        tetengo_lattice_nBestIterator_next(p_iterator);
        BOOST_TEST(tetengo_lattice_nBestIterator_hasNext(p_iterator));

        tetengo_lattice_nBestIterator_next(p_iterator);
        BOOST_TEST(tetengo_lattice_nBestIterator_hasNext(p_iterator));

        tetengo_lattice_nBestIterator_next(p_iterator);
        BOOST_TEST(tetengo_lattice_nBestIterator_hasNext(p_iterator));

        tetengo_lattice_nBestIterator_next(p_iterator);
        BOOST_TEST(tetengo_lattice_nBestIterator_hasNext(p_iterator));

        tetengo_lattice_nBestIterator_next(p_iterator);
        BOOST_TEST(!tetengo_lattice_nBestIterator_hasNext(p_iterator));
    }
    {
        BOOST_TEST(!tetengo_lattice_nBestIterator_hasNext(nullptr));
    }
}

BOOST_AUTO_TEST_CASE(operator_increment)
{
    BOOST_TEST_PASSPOINT();

    {
        const auto                p_vocabulary = create_cpp_vocabulary();
        tetengo::lattice::lattice lattice_{ *p_vocabulary };
        lattice_.push_back(to_input("[HakataTosu]"));
        lattice_.push_back(to_input("[TosuOmuta]"));
        lattice_.push_back(to_input("[OmutaKumamoto]"));

        auto                              eos_node_and_preceding_edge_costs = lattice_.settle();
        tetengo::lattice::n_best_iterator iterator{ lattice_,
                                                    std::move(eos_node_and_preceding_edge_costs.first),
                                                    std::make_unique<tetengo::lattice::constraint>() };
        {
            const auto& path = *iterator;
            BOOST_TEST_REQUIRE(std::size(path.nodes()) == 3U);
            BOOST_TEST(!path.nodes()[0].value().has_value());
            BOOST_TEST(std::any_cast<std::string>(path.nodes()[1].value()) == "tsubame");
            BOOST_TEST(preceding_edge_cost(path, 1) == 600);
            BOOST_TEST(!path.nodes()[2].value().has_value());
            BOOST_TEST(preceding_edge_cost(path, 2) == 400);
            BOOST_TEST(recalc_path_cost(path) == path.cost());
        }

        ++iterator;
        {
            const auto& path = *iterator;
            BOOST_TEST_REQUIRE(std::size(path.nodes()) == 3U);
            BOOST_TEST(!path.nodes()[0].value().has_value());
            BOOST_TEST(std::any_cast<std::string>(path.nodes()[1].value()) == "sakura");
            BOOST_TEST(preceding_edge_cost(path, 1) == 600);
            BOOST_TEST(!path.nodes()[2].value().has_value());
            BOOST_TEST(preceding_edge_cost(path, 2) == 400);
            BOOST_TEST(recalc_path_cost(path) == path.cost());
        }

        iterator++;
        {
            const auto& path = *iterator;
            BOOST_TEST_REQUIRE(std::size(path.nodes()) == 4U);
            BOOST_TEST(!path.nodes()[0].value().has_value());
            BOOST_TEST(std::any_cast<std::string>(path.nodes()[1].value()) == "rapid811");
            BOOST_TEST(preceding_edge_cost(path, 1) == 700);
            BOOST_TEST(std::any_cast<std::string>(path.nodes()[2].value()) == "local817");
            BOOST_TEST(preceding_edge_cost(path, 2) == 200);
            BOOST_TEST(!path.nodes()[3].value().has_value());
            BOOST_TEST(preceding_edge_cost(path, 3) == 600);
            BOOST_TEST(recalc_path_cost(path) == path.cost());
        }

        ++iterator;
        {
            const auto& path = *iterator;
            BOOST_TEST_REQUIRE(std::size(path.nodes()) == 4U);
            BOOST_TEST(!path.nodes()[0].value().has_value());
            BOOST_TEST(std::any_cast<std::string>(path.nodes()[1].value()) == "local415");
            BOOST_TEST(preceding_edge_cost(path, 1) == 800);
            BOOST_TEST(std::any_cast<std::string>(path.nodes()[2].value()) == "local815");
            BOOST_TEST(preceding_edge_cost(path, 2) == 500);
            BOOST_TEST(!path.nodes()[3].value().has_value());
            BOOST_TEST(preceding_edge_cost(path, 3) == 500);
            BOOST_TEST(recalc_path_cost(path) == path.cost());
        }

        iterator++;
        {
            const auto& path = *iterator;
            BOOST_TEST_REQUIRE(std::size(path.nodes()) == 4U);
            BOOST_TEST(!path.nodes()[0].value().has_value());
            BOOST_TEST(std::any_cast<std::string>(path.nodes()[1].value()) == "kamome");
            BOOST_TEST(preceding_edge_cost(path, 1) == 800);
            BOOST_TEST(std::any_cast<std::string>(path.nodes()[2].value()) == "local815");
            BOOST_TEST(preceding_edge_cost(path, 2) == 500);
            BOOST_TEST(!path.nodes()[3].value().has_value());
            BOOST_TEST(preceding_edge_cost(path, 3) == 500);
            BOOST_TEST(recalc_path_cost(path) == path.cost());
        }

        ++iterator;
        {
            const auto& path = *iterator;
            BOOST_TEST_REQUIRE(std::size(path.nodes()) == 4U);
            BOOST_TEST(!path.nodes()[0].value().has_value());
            BOOST_TEST(std::any_cast<std::string>(path.nodes()[1].value()) == "ariake");
            BOOST_TEST(preceding_edge_cost(path, 1) == 700);
            BOOST_TEST(std::any_cast<std::string>(path.nodes()[2].value()) == "local817");
            BOOST_TEST(preceding_edge_cost(path, 2) == 200);
            BOOST_TEST(!path.nodes()[3].value().has_value());
            BOOST_TEST(preceding_edge_cost(path, 3) == 600);
            BOOST_TEST(recalc_path_cost(path) == path.cost());
        }

        iterator++;
        {
            const auto& path = *iterator;
            BOOST_TEST_REQUIRE(std::size(path.nodes()) == 3U);
            BOOST_TEST(!path.nodes()[0].value().has_value());
            BOOST_TEST(std::any_cast<std::string>(path.nodes()[1].value()) == "mizuho");
            BOOST_TEST(preceding_edge_cost(path, 1) == 600);
            BOOST_TEST(!path.nodes()[2].value().has_value());
            BOOST_TEST(preceding_edge_cost(path, 2) == 400);
            BOOST_TEST(recalc_path_cost(path) == path.cost());
        }

        ++iterator;
        {
            const auto& path = *iterator;
            BOOST_TEST_REQUIRE(std::size(path.nodes()) == 5U);
            BOOST_TEST(!path.nodes()[0].value().has_value());
            BOOST_TEST(std::any_cast<std::string>(path.nodes()[1].value()) == "local415");
            BOOST_TEST(preceding_edge_cost(path, 1) == 800);
            BOOST_TEST(std::any_cast<std::string>(path.nodes()[2].value()) == "local813");
            BOOST_TEST(preceding_edge_cost(path, 2) == 600);
            BOOST_TEST(std::any_cast<std::string>(path.nodes()[3].value()) == "local817");
            BOOST_TEST(preceding_edge_cost(path, 3) == 300);
            BOOST_TEST(!path.nodes()[4].value().has_value());
            BOOST_TEST(preceding_edge_cost(path, 4) == 600);
            BOOST_TEST(recalc_path_cost(path) == path.cost());
        }

        iterator++;
        {
            const auto& path = *iterator;
            BOOST_TEST_REQUIRE(std::size(path.nodes()) == 5U);
            BOOST_TEST(!path.nodes()[0].value().has_value());
            BOOST_TEST(std::any_cast<std::string>(path.nodes()[1].value()) == "kamome");
            BOOST_TEST(preceding_edge_cost(path, 1) == 800);
            BOOST_TEST(std::any_cast<std::string>(path.nodes()[2].value()) == "local813");
            BOOST_TEST(preceding_edge_cost(path, 2) == 600);
            BOOST_TEST(std::any_cast<std::string>(path.nodes()[3].value()) == "local817");
            BOOST_TEST(preceding_edge_cost(path, 3) == 300);
            BOOST_TEST(!path.nodes()[4].value().has_value());
            BOOST_TEST(preceding_edge_cost(path, 4) == 600);
            BOOST_TEST(recalc_path_cost(path) == path.cost());
        }

        ++iterator;
        BOOST_CHECK_THROW([[maybe_unused]] const auto& dereferenced = *iterator, std::logic_error);
    }
    {
        const auto                p_vocabulary = create_cpp_vocabulary();
        tetengo::lattice::lattice lattice_{ *p_vocabulary };
        lattice_.push_back(to_input("[HakataTosu]"));
        lattice_.push_back(to_input("[TosuOmuta]"));
        lattice_.push_back(to_input("[OmutaKumamoto]"));

        auto                              eos_node_and_preceding_edge_costs = lattice_.settle();
        tetengo::lattice::n_best_iterator iterator{ lattice_,
                                                    std::move(eos_node_and_preceding_edge_costs.first),
                                                    std::make_unique<tetengo::lattice::constraint>() };

        {
            const auto& path = *iterator;
            BOOST_TEST_REQUIRE(std::size(path.nodes()) == 3U);

            std::vector<std::unique_ptr<tetengo::lattice::constraint_element>> pattern{};
            pattern.reserve(3);
            pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path.nodes()[0]));
            pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path.nodes()[1]));
            pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path.nodes()[2]));
            auto p_constraint = std::make_unique<tetengo::lattice::constraint>(std::move(pattern));

            tetengo::lattice::n_best_iterator constrained_iterator{ lattice_,
                                                                    std::move(eos_node_and_preceding_edge_costs.first),
                                                                    std::move(p_constraint) };

            BOOST_REQUIRE(constrained_iterator != tetengo::lattice::n_best_iterator{});
            const auto& constrained_path = *iterator;
            BOOST_TEST(constrained_path.nodes() == path.nodes());

            ++constrained_iterator;
            BOOST_CHECK(constrained_iterator == tetengo::lattice::n_best_iterator{});
        }

        ++iterator;
        iterator++;
        {
            const auto& path = *iterator;
            BOOST_TEST_REQUIRE(std::size(path.nodes()) == 4U);

            std::vector<std::unique_ptr<tetengo::lattice::constraint_element>> pattern{};
            pattern.reserve(4);
            pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path.nodes()[0]));
            pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path.nodes()[1]));
            pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path.nodes()[2]));
            pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path.nodes()[3]));
            auto p_constraint = std::make_unique<tetengo::lattice::constraint>(std::move(pattern));

            tetengo::lattice::n_best_iterator constrained_iterator{ lattice_,
                                                                    std::move(eos_node_and_preceding_edge_costs.first),
                                                                    std::move(p_constraint) };

            BOOST_REQUIRE(constrained_iterator != tetengo::lattice::n_best_iterator{});
            const auto& constrained_path = *constrained_iterator;
            BOOST_TEST(constrained_path.nodes() == path.nodes());

            constrained_iterator++;
            BOOST_CHECK(constrained_iterator == tetengo::lattice::n_best_iterator{});
        }

        ++iterator;
        iterator++;
        {
            const auto& path = *iterator;
            BOOST_TEST_REQUIRE(std::size(path.nodes()) == 4U);

            std::vector<std::unique_ptr<tetengo::lattice::constraint_element>> pattern{};
            pattern.reserve(4);
            pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path.nodes()[0]));
            pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path.nodes()[1]));
            pattern.push_back(std::make_unique<tetengo::lattice::wildcard_constraint_element>(1));
            pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path.nodes()[3]));
            auto p_constraint = std::make_unique<tetengo::lattice::constraint>(std::move(pattern));

            tetengo::lattice::n_best_iterator constrained_iterator{ lattice_,
                                                                    std::move(eos_node_and_preceding_edge_costs.first),
                                                                    std::move(p_constraint) };

            {
                BOOST_REQUIRE(constrained_iterator != tetengo::lattice::n_best_iterator{});
                const auto& constrained_path = *constrained_iterator;
                BOOST_TEST(constrained_path.nodes() == path.nodes());
            }
            ++constrained_iterator;
            {
                BOOST_REQUIRE(constrained_iterator != tetengo::lattice::n_best_iterator{});
                const auto& constrained_path = *constrained_iterator;
                BOOST_TEST_REQUIRE(std::size(constrained_path.nodes()) == 5U);
                BOOST_CHECK(constrained_path.nodes()[0] == path.nodes()[0]);
                BOOST_CHECK(constrained_path.nodes()[1] == path.nodes()[1]);
                BOOST_TEST_REQUIRE(constrained_path.nodes()[2].p_key());
                BOOST_TEST_REQUIRE(constrained_path.nodes()[2].p_key()->is<tetengo::lattice::string_input>());
                BOOST_TEST(
                    constrained_path.nodes()[2].p_key()->as<tetengo::lattice::string_input>().value() == "Tosu-Omuta");
                BOOST_TEST_REQUIRE(constrained_path.nodes()[3].p_key());
                BOOST_TEST_REQUIRE(constrained_path.nodes()[3].p_key()->is<tetengo::lattice::string_input>());
                BOOST_TEST(
                    constrained_path.nodes()[3].p_key()->as<tetengo::lattice::string_input>().value() ==
                    "Omuta-Kumamoto");
                BOOST_CHECK(constrained_path.nodes()[4] == path.nodes()[3]);
            }
            constrained_iterator++;
            {
                BOOST_CHECK(constrained_iterator == tetengo::lattice::n_best_iterator{});
            }
        }
        {
            const auto& path = *iterator;
            BOOST_TEST_REQUIRE(std::size(path.nodes()) == 4U);

            std::vector<std::unique_ptr<tetengo::lattice::constraint_element>> pattern{};
            pattern.reserve(4);
            pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path.nodes()[0]));
            pattern.push_back(std::make_unique<tetengo::lattice::wildcard_constraint_element>(0));
            pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path.nodes()[2]));
            pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path.nodes()[3]));
            auto p_constraint = std::make_unique<tetengo::lattice::constraint>(std::move(pattern));

            tetengo::lattice::n_best_iterator constrained_iterator{ lattice_,
                                                                    std::move(eos_node_and_preceding_edge_costs.first),
                                                                    std::move(p_constraint) };

            {
                BOOST_REQUIRE(constrained_iterator != tetengo::lattice::n_best_iterator{});
                const auto& constrained_path = *constrained_iterator;
                BOOST_TEST_REQUIRE(std::size(constrained_path.nodes()) == 4U);
                BOOST_CHECK(constrained_path.nodes()[0] == path.nodes()[0]);
                BOOST_CHECK(std::any_cast<std::string>(constrained_path.nodes()[1].value()) == "local415");
                BOOST_CHECK(constrained_path.nodes()[2] == path.nodes()[2]);
                BOOST_CHECK(constrained_path.nodes()[3] == path.nodes()[3]);
            }
            ++constrained_iterator;
            {
                BOOST_REQUIRE(constrained_iterator != tetengo::lattice::n_best_iterator{});
                const auto& constrained_path = *constrained_iterator;
                BOOST_TEST(constrained_path.nodes() == path.nodes());
            }
            constrained_iterator++;
            {
                BOOST_CHECK(constrained_iterator == tetengo::lattice::n_best_iterator{});
            }
        }
        {
            const auto& path = *iterator;
            BOOST_TEST_REQUIRE(std::size(path.nodes()) == 4U);

            std::vector<std::unique_ptr<tetengo::lattice::constraint_element>> pattern{};
            pattern.reserve(4);
            pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path.nodes()[0]));
            pattern.push_back(std::make_unique<tetengo::lattice::wildcard_constraint_element>(0));
            pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path.nodes()[3]));
            auto p_constraint = std::make_unique<tetengo::lattice::constraint>(std::move(pattern));

            tetengo::lattice::n_best_iterator constrained_iterator{ lattice_,
                                                                    std::move(eos_node_and_preceding_edge_costs.first),
                                                                    std::move(p_constraint) };

            BOOST_TEST(std::distance(constrained_iterator, tetengo::lattice::n_best_iterator{}) == 9);
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

        const auto             preceding_edge_cost_count = tetengo_lattice_lattice_settle(p_lattice, nullptr, nullptr);
        tetengo_lattice_node_t eos_node{};
        std::vector<int>       preceding_edge_costs(preceding_edge_cost_count, 0);
        tetengo_lattice_lattice_settle(p_lattice, &eos_node, std::data(preceding_edge_costs));
        auto* const p_iterator =
            tetengo_lattice_nBestIterator_create(p_lattice, &eos_node, tetengo_lattice_constraint_createEmpty());
        BOOST_SCOPE_EXIT(p_iterator)
        {
            tetengo_lattice_nBestIterator_destroy(p_iterator);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_iterator);

        {
            const auto* const p_path = tetengo_lattice_nBestIterator_createPath(p_iterator);
            BOOST_SCOPE_EXIT(p_path)
            {
                tetengo_lattice_path_destroy(p_path);
            }
            BOOST_SCOPE_EXIT_END;
            BOOST_TEST_REQUIRE(p_path);

            const auto node_count = tetengo_lattice_path_pNodes(p_path, nullptr);
            BOOST_TEST_REQUIRE(node_count == 3U);
            std::vector<tetengo_lattice_node_t> nodes{ node_count };
            const auto node_count_again = tetengo_lattice_path_pNodes(p_path, std::data(nodes));
            BOOST_TEST(node_count_again == 3U);
            BOOST_TEST(!tetengo_lattice_entryView_valueOf(nodes[0].value_handle));
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(nodes[1].value_handle)) ==
                "tsubame");
            BOOST_TEST(c_preceding_edge_cost(nodes, 1) == 600);
            BOOST_TEST(!tetengo_lattice_entryView_valueOf(nodes[2].value_handle));
            BOOST_TEST(c_preceding_edge_cost(nodes, 2) == 400);
            BOOST_TEST(recalc_c_path_cost(nodes) == tetengo_lattice_path_cost(p_path));
        }

        tetengo_lattice_nBestIterator_next(p_iterator);
        {
            const auto* const p_path = tetengo_lattice_nBestIterator_createPath(p_iterator);
            BOOST_SCOPE_EXIT(p_path)
            {
                tetengo_lattice_path_destroy(p_path);
            }
            BOOST_SCOPE_EXIT_END;
            BOOST_TEST_REQUIRE(p_path);

            const auto node_count = tetengo_lattice_path_pNodes(p_path, nullptr);
            BOOST_TEST_REQUIRE(node_count == 3U);
            std::vector<tetengo_lattice_node_t> nodes{ node_count };
            const auto node_count_again = tetengo_lattice_path_pNodes(p_path, std::data(nodes));
            BOOST_TEST(node_count_again == 3U);
            BOOST_TEST(!tetengo_lattice_entryView_valueOf(nodes[0].value_handle));
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(nodes[1].value_handle)) ==
                "sakura");
            BOOST_TEST(c_preceding_edge_cost(nodes, 1) == 600);
            BOOST_TEST(!tetengo_lattice_entryView_valueOf(nodes[2].value_handle));
            BOOST_TEST(c_preceding_edge_cost(nodes, 2) == 400);
            BOOST_TEST(recalc_c_path_cost(nodes) == tetengo_lattice_path_cost(p_path));
        }

        tetengo_lattice_nBestIterator_next(p_iterator);
        {
            const auto* const p_path = tetengo_lattice_nBestIterator_createPath(p_iterator);
            BOOST_SCOPE_EXIT(p_path)
            {
                tetengo_lattice_path_destroy(p_path);
            }
            BOOST_SCOPE_EXIT_END;
            BOOST_TEST_REQUIRE(p_path);

            const auto node_count = tetengo_lattice_path_pNodes(p_path, nullptr);
            BOOST_TEST_REQUIRE(node_count == 4U);
            std::vector<tetengo_lattice_node_t> nodes{ node_count };
            const auto node_count_again = tetengo_lattice_path_pNodes(p_path, std::data(nodes));
            BOOST_TEST(node_count_again == 4U);
            BOOST_TEST(!tetengo_lattice_entryView_valueOf(nodes[0].value_handle));
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(nodes[1].value_handle)) ==
                "rapid811");
            BOOST_TEST(c_preceding_edge_cost(nodes, 1) == 700);
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(nodes[2].value_handle)) ==
                "local817");
            BOOST_TEST(c_preceding_edge_cost(nodes, 2) == 200);
            BOOST_TEST(!tetengo_lattice_entryView_valueOf(nodes[3].value_handle));
            BOOST_TEST(c_preceding_edge_cost(nodes, 3) == 600);
            BOOST_TEST(recalc_c_path_cost(nodes) == tetengo_lattice_path_cost(p_path));
        }

        tetengo_lattice_nBestIterator_next(p_iterator);
        {
            const auto* const p_path = tetengo_lattice_nBestIterator_createPath(p_iterator);
            BOOST_SCOPE_EXIT(p_path)
            {
                tetengo_lattice_path_destroy(p_path);
            }
            BOOST_SCOPE_EXIT_END;
            BOOST_TEST_REQUIRE(p_path);

            const auto node_count = tetengo_lattice_path_pNodes(p_path, nullptr);
            BOOST_TEST_REQUIRE(node_count == 4U);
            std::vector<tetengo_lattice_node_t> nodes{ node_count };
            const auto node_count_again = tetengo_lattice_path_pNodes(p_path, std::data(nodes));
            BOOST_TEST(node_count_again == 4U);
            BOOST_TEST(!tetengo_lattice_entryView_valueOf(nodes[0].value_handle));
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(nodes[1].value_handle)) ==
                "local415");
            BOOST_TEST(c_preceding_edge_cost(nodes, 1) == 800);
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(nodes[2].value_handle)) ==
                "local815");
            BOOST_TEST(c_preceding_edge_cost(nodes, 2) == 500);
            BOOST_TEST(!tetengo_lattice_entryView_valueOf(nodes[3].value_handle));
            BOOST_TEST(c_preceding_edge_cost(nodes, 3) == 500);
            BOOST_TEST(recalc_c_path_cost(nodes) == tetengo_lattice_path_cost(p_path));
        }

        tetengo_lattice_nBestIterator_next(p_iterator);
        {
            const auto* const p_path = tetengo_lattice_nBestIterator_createPath(p_iterator);
            BOOST_SCOPE_EXIT(p_path)
            {
                tetengo_lattice_path_destroy(p_path);
            }
            BOOST_SCOPE_EXIT_END;
            BOOST_TEST_REQUIRE(p_path);

            const auto node_count = tetengo_lattice_path_pNodes(p_path, nullptr);
            BOOST_TEST_REQUIRE(node_count == 4U);
            std::vector<tetengo_lattice_node_t> nodes{ node_count };
            const auto node_count_again = tetengo_lattice_path_pNodes(p_path, std::data(nodes));
            BOOST_TEST(node_count_again == 4U);
            BOOST_TEST(!tetengo_lattice_entryView_valueOf(nodes[0].value_handle));
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(nodes[1].value_handle)) ==
                "kamome");
            BOOST_TEST(c_preceding_edge_cost(nodes, 1) == 800);
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(nodes[2].value_handle)) ==
                "local815");
            BOOST_TEST(c_preceding_edge_cost(nodes, 2) == 500);
            BOOST_TEST(!tetengo_lattice_entryView_valueOf(nodes[3].value_handle));
            BOOST_TEST(c_preceding_edge_cost(nodes, 3) == 500);
            BOOST_TEST(recalc_c_path_cost(nodes) == tetengo_lattice_path_cost(p_path));
        }

        tetengo_lattice_nBestIterator_next(p_iterator);
        {
            const auto* const p_path = tetengo_lattice_nBestIterator_createPath(p_iterator);
            BOOST_SCOPE_EXIT(p_path)
            {
                tetengo_lattice_path_destroy(p_path);
            }
            BOOST_SCOPE_EXIT_END;
            BOOST_TEST_REQUIRE(p_path);

            const auto node_count = tetengo_lattice_path_pNodes(p_path, nullptr);
            BOOST_TEST_REQUIRE(node_count == 4U);
            std::vector<tetengo_lattice_node_t> nodes{ node_count };
            const auto node_count_again = tetengo_lattice_path_pNodes(p_path, std::data(nodes));
            BOOST_TEST(node_count_again == 4U);
            BOOST_TEST(!tetengo_lattice_entryView_valueOf(nodes[0].value_handle));
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(nodes[1].value_handle)) ==
                "ariake");
            BOOST_TEST(c_preceding_edge_cost(nodes, 1) == 700);
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(nodes[2].value_handle)) ==
                "local817");
            BOOST_TEST(c_preceding_edge_cost(nodes, 2) == 200);
            BOOST_TEST(!tetengo_lattice_entryView_valueOf(nodes[3].value_handle));
            BOOST_TEST(c_preceding_edge_cost(nodes, 3) == 600);
            BOOST_TEST(recalc_c_path_cost(nodes) == tetengo_lattice_path_cost(p_path));
        }

        tetengo_lattice_nBestIterator_next(p_iterator);
        {
            const auto* const p_path = tetengo_lattice_nBestIterator_createPath(p_iterator);
            BOOST_SCOPE_EXIT(p_path)
            {
                tetengo_lattice_path_destroy(p_path);
            }
            BOOST_SCOPE_EXIT_END;
            BOOST_TEST_REQUIRE(p_path);

            const auto node_count = tetengo_lattice_path_pNodes(p_path, nullptr);
            BOOST_TEST_REQUIRE(node_count == 3U);
            std::vector<tetengo_lattice_node_t> nodes{ node_count };
            const auto node_count_again = tetengo_lattice_path_pNodes(p_path, std::data(nodes));
            BOOST_TEST(node_count_again == 3U);
            BOOST_TEST(!tetengo_lattice_entryView_valueOf(nodes[0].value_handle));
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(nodes[1].value_handle)) ==
                "mizuho");
            BOOST_TEST(c_preceding_edge_cost(nodes, 1) == 600);
            BOOST_TEST(!tetengo_lattice_entryView_valueOf(nodes[2].value_handle));
            BOOST_TEST(c_preceding_edge_cost(nodes, 2) == 400);
            BOOST_TEST(recalc_c_path_cost(nodes) == tetengo_lattice_path_cost(p_path));
        }

        tetengo_lattice_nBestIterator_next(p_iterator);
        {
            const auto* const p_path = tetengo_lattice_nBestIterator_createPath(p_iterator);
            BOOST_SCOPE_EXIT(p_path)
            {
                tetengo_lattice_path_destroy(p_path);
            }
            BOOST_SCOPE_EXIT_END;
            BOOST_TEST_REQUIRE(p_path);

            const auto node_count = tetengo_lattice_path_pNodes(p_path, nullptr);
            BOOST_TEST_REQUIRE(node_count == 5U);
            std::vector<tetengo_lattice_node_t> nodes{ node_count };
            const auto node_count_again = tetengo_lattice_path_pNodes(p_path, std::data(nodes));
            BOOST_TEST(node_count_again == 5U);
            BOOST_TEST(!tetengo_lattice_entryView_valueOf(nodes[0].value_handle));
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(nodes[1].value_handle)) ==
                "local415");
            BOOST_TEST(c_preceding_edge_cost(nodes, 1) == 800);
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(nodes[2].value_handle)) ==
                "local813");
            BOOST_TEST(c_preceding_edge_cost(nodes, 2) == 600);
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(nodes[3].value_handle)) ==
                "local817");
            BOOST_TEST(c_preceding_edge_cost(nodes, 3) == 300);
            BOOST_TEST(!tetengo_lattice_entryView_valueOf(nodes[4].value_handle));
            BOOST_TEST(c_preceding_edge_cost(nodes, 4) == 600);
            BOOST_TEST(recalc_c_path_cost(nodes) == tetengo_lattice_path_cost(p_path));
        }

        tetengo_lattice_nBestIterator_next(p_iterator);
        {
            const auto* const p_path = tetengo_lattice_nBestIterator_createPath(p_iterator);
            BOOST_SCOPE_EXIT(p_path)
            {
                tetengo_lattice_path_destroy(p_path);
            }
            BOOST_SCOPE_EXIT_END;
            BOOST_TEST_REQUIRE(p_path);

            const auto node_count = tetengo_lattice_path_pNodes(p_path, nullptr);
            BOOST_TEST_REQUIRE(node_count == 5U);
            std::vector<tetengo_lattice_node_t> nodes{ node_count };
            const auto node_count_again = tetengo_lattice_path_pNodes(p_path, std::data(nodes));
            BOOST_TEST(node_count_again == 5U);
            BOOST_TEST(!tetengo_lattice_entryView_valueOf(nodes[0].value_handle));
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(nodes[1].value_handle)) ==
                "kamome");
            BOOST_TEST(c_preceding_edge_cost(nodes, 1) == 800);
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(nodes[2].value_handle)) ==
                "local813");
            BOOST_TEST(c_preceding_edge_cost(nodes, 2) == 600);
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entryView_valueOf(nodes[3].value_handle)) ==
                "local817");
            BOOST_TEST(c_preceding_edge_cost(nodes, 3) == 300);
            BOOST_TEST(!tetengo_lattice_entryView_valueOf(nodes[4].value_handle));
            BOOST_TEST(c_preceding_edge_cost(nodes, 4) == 600);
            BOOST_TEST(recalc_c_path_cost(nodes) == tetengo_lattice_path_cost(p_path));
        }

        tetengo_lattice_nBestIterator_next(p_iterator);
        {
            BOOST_TEST(!tetengo_lattice_nBestIterator_hasNext(p_iterator));
            const auto* const p_path = tetengo_lattice_nBestIterator_createPath(p_iterator);
            BOOST_TEST(!p_path);
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

        const auto             preceding_edge_cost_count = tetengo_lattice_lattice_settle(p_lattice, nullptr, nullptr);
        tetengo_lattice_node_t eos_node{};
        std::vector<int>       preceding_edge_costs(preceding_edge_cost_count, 0);
        tetengo_lattice_lattice_settle(p_lattice, &eos_node, std::data(preceding_edge_costs));
        auto* const p_iterator =
            tetengo_lattice_nBestIterator_create(p_lattice, &eos_node, tetengo_lattice_constraint_createEmpty());
        BOOST_SCOPE_EXIT(p_iterator)
        {
            tetengo_lattice_nBestIterator_destroy(p_iterator);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_iterator);

        {
            const auto* const p_path = tetengo_lattice_nBestIterator_createPath(p_iterator);
            BOOST_SCOPE_EXIT(p_path)
            {
                tetengo_lattice_path_destroy(p_path);
            }
            BOOST_SCOPE_EXIT_END;
            BOOST_TEST_REQUIRE(p_path);

            const auto node_count = tetengo_lattice_path_pNodes(p_path, nullptr);
            BOOST_TEST_REQUIRE(node_count == 3U);
            std::vector<tetengo_lattice_node_t> nodes{ node_count };
            const auto node_count_again = tetengo_lattice_path_pNodes(p_path, std::data(nodes));
            BOOST_TEST(node_count_again == 3U);

            std::vector<tetengo_lattice_constraintElement_t*> pattern{};
            pattern.reserve(3);
            pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&nodes[0]));
            pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&nodes[1]));
            pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&nodes[2]));

            auto* const p_constrained_iterator = tetengo_lattice_nBestIterator_create(
                p_lattice, &eos_node, tetengo_lattice_constraint_create(std::data(pattern), std::size(pattern)));
            BOOST_SCOPE_EXIT(p_constrained_iterator)
            {
                tetengo_lattice_nBestIterator_destroy(p_constrained_iterator);
            }
            BOOST_SCOPE_EXIT_END;
            BOOST_TEST_REQUIRE(p_constrained_iterator);

            BOOST_TEST_REQUIRE(tetengo_lattice_nBestIterator_hasNext(p_constrained_iterator));
            const auto* const p_constrained_path = tetengo_lattice_nBestIterator_createPath(p_constrained_iterator);
            BOOST_SCOPE_EXIT(p_constrained_path)
            {
                tetengo_lattice_path_destroy(p_constrained_path);
            }
            BOOST_SCOPE_EXIT_END;
            BOOST_TEST_REQUIRE(p_constrained_path);

            const auto constrained_node_count = tetengo_lattice_path_pNodes(p_constrained_path, nullptr);
            BOOST_TEST_REQUIRE(constrained_node_count == 3U);
            std::vector<tetengo_lattice_node_t> constrained_nodes{ constrained_node_count };
            const auto                          constrained_node_count_again =
                tetengo_lattice_path_pNodes(p_constrained_path, std::data(constrained_nodes));
            BOOST_TEST(constrained_node_count_again == 3U);
            BOOST_TEST(
                tetengo_lattice_entryView_valueOf(constrained_nodes[0].value_handle) ==
                tetengo_lattice_entryView_valueOf(nodes[0].value_handle));
            BOOST_TEST(
                tetengo_lattice_entryView_valueOf(constrained_nodes[1].value_handle) ==
                tetengo_lattice_entryView_valueOf(nodes[1].value_handle));
            BOOST_TEST(
                tetengo_lattice_entryView_valueOf(constrained_nodes[2].value_handle) ==
                tetengo_lattice_entryView_valueOf(nodes[2].value_handle));

            tetengo_lattice_nBestIterator_next(p_constrained_iterator);
            BOOST_TEST(!tetengo_lattice_nBestIterator_hasNext(p_constrained_iterator));
        }

        tetengo_lattice_nBestIterator_next(p_iterator);
        tetengo_lattice_nBestIterator_next(p_iterator);
        {
            const auto* const p_path = tetengo_lattice_nBestIterator_createPath(p_iterator);
            BOOST_SCOPE_EXIT(p_path)
            {
                tetengo_lattice_path_destroy(p_path);
            }
            BOOST_SCOPE_EXIT_END;
            BOOST_TEST_REQUIRE(p_path);

            const auto node_count = tetengo_lattice_path_pNodes(p_path, nullptr);
            BOOST_TEST_REQUIRE(node_count == 4U);
            std::vector<tetengo_lattice_node_t> nodes{ node_count };
            const auto node_count_again = tetengo_lattice_path_pNodes(p_path, std::data(nodes));
            BOOST_TEST(node_count_again == 4U);

            std::vector<tetengo_lattice_constraintElement_t*> pattern{};
            pattern.reserve(4);
            pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&nodes[0]));
            pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&nodes[1]));
            pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&nodes[2]));
            pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&nodes[3]));

            auto* const p_constrained_iterator = tetengo_lattice_nBestIterator_create(
                p_lattice, &eos_node, tetengo_lattice_constraint_create(std::data(pattern), std::size(pattern)));
            BOOST_SCOPE_EXIT(p_constrained_iterator)
            {
                tetengo_lattice_nBestIterator_destroy(p_constrained_iterator);
            }
            BOOST_SCOPE_EXIT_END;
            BOOST_TEST_REQUIRE(p_constrained_iterator);

            BOOST_TEST_REQUIRE(tetengo_lattice_nBestIterator_hasNext(p_constrained_iterator));
            BOOST_TEST_REQUIRE(tetengo_lattice_nBestIterator_hasNext(p_constrained_iterator));
            const auto* const p_constrained_path = tetengo_lattice_nBestIterator_createPath(p_constrained_iterator);
            BOOST_SCOPE_EXIT(p_constrained_path)
            {
                tetengo_lattice_path_destroy(p_constrained_path);
            }
            BOOST_SCOPE_EXIT_END;
            BOOST_TEST_REQUIRE(p_constrained_path);

            const auto constrained_node_count = tetengo_lattice_path_pNodes(p_constrained_path, nullptr);
            BOOST_TEST_REQUIRE(constrained_node_count == 4U);
            std::vector<tetengo_lattice_node_t> constrained_nodes{ constrained_node_count };
            const auto                          constrained_node_count_again =
                tetengo_lattice_path_pNodes(p_constrained_path, std::data(constrained_nodes));
            BOOST_TEST(constrained_node_count_again == 4U);
            BOOST_TEST(
                tetengo_lattice_entryView_valueOf(constrained_nodes[0].value_handle) ==
                tetengo_lattice_entryView_valueOf(nodes[0].value_handle));
            BOOST_TEST(
                tetengo_lattice_entryView_valueOf(constrained_nodes[1].value_handle) ==
                tetengo_lattice_entryView_valueOf(nodes[1].value_handle));
            BOOST_TEST(
                tetengo_lattice_entryView_valueOf(constrained_nodes[2].value_handle) ==
                tetengo_lattice_entryView_valueOf(nodes[2].value_handle));
            BOOST_TEST(
                tetengo_lattice_entryView_valueOf(constrained_nodes[3].value_handle) ==
                tetengo_lattice_entryView_valueOf(nodes[3].value_handle));

            tetengo_lattice_nBestIterator_next(p_constrained_iterator);
            BOOST_TEST(!tetengo_lattice_nBestIterator_hasNext(p_constrained_iterator));
        }

        tetengo_lattice_nBestIterator_next(p_iterator);
        tetengo_lattice_nBestIterator_next(p_iterator);
        {
            const auto* const p_path = tetengo_lattice_nBestIterator_createPath(p_iterator);
            BOOST_SCOPE_EXIT(p_path)
            {
                tetengo_lattice_path_destroy(p_path);
            }
            BOOST_SCOPE_EXIT_END;
            BOOST_TEST_REQUIRE(p_path);

            const auto node_count = tetengo_lattice_path_pNodes(p_path, nullptr);
            BOOST_TEST_REQUIRE(node_count == 4U);
            std::vector<tetengo_lattice_node_t> nodes{ node_count };
            const auto node_count_again = tetengo_lattice_path_pNodes(p_path, std::data(nodes));
            BOOST_TEST(node_count_again == 4U);

            std::vector<tetengo_lattice_constraintElement_t*> pattern{};
            pattern.reserve(4);
            pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&nodes[0]));
            pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&nodes[1]));
            pattern.push_back(tetengo_lattice_constraintElement_createWildcardConstraintElement(1));
            pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&nodes[3]));

            auto* const p_constrained_iterator = tetengo_lattice_nBestIterator_create(
                p_lattice, &eos_node, tetengo_lattice_constraint_create(std::data(pattern), std::size(pattern)));
            BOOST_SCOPE_EXIT(p_constrained_iterator)
            {
                tetengo_lattice_nBestIterator_destroy(p_constrained_iterator);
            }
            BOOST_SCOPE_EXIT_END;
            BOOST_TEST_REQUIRE(p_constrained_iterator);

            {
                BOOST_TEST_REQUIRE(tetengo_lattice_nBestIterator_hasNext(p_constrained_iterator));
                BOOST_TEST_REQUIRE(tetengo_lattice_nBestIterator_hasNext(p_constrained_iterator));
                const auto* const p_constrained_path = tetengo_lattice_nBestIterator_createPath(p_constrained_iterator);
                BOOST_SCOPE_EXIT(p_constrained_path)
                {
                    tetengo_lattice_path_destroy(p_constrained_path);
                }
                BOOST_SCOPE_EXIT_END;
                BOOST_TEST_REQUIRE(p_constrained_path);

                const auto constrained_node_count = tetengo_lattice_path_pNodes(p_constrained_path, nullptr);
                BOOST_TEST_REQUIRE(constrained_node_count == 4U);
                std::vector<tetengo_lattice_node_t> constrained_nodes{ constrained_node_count };
                const auto                          constrained_node_count_again =
                    tetengo_lattice_path_pNodes(p_constrained_path, std::data(constrained_nodes));
                BOOST_TEST(constrained_node_count_again == 4U);
                BOOST_TEST(
                    tetengo_lattice_entryView_valueOf(constrained_nodes[0].value_handle) ==
                    tetengo_lattice_entryView_valueOf(nodes[0].value_handle));
                BOOST_TEST(
                    tetengo_lattice_entryView_valueOf(constrained_nodes[1].value_handle) ==
                    tetengo_lattice_entryView_valueOf(nodes[1].value_handle));
                BOOST_TEST(
                    tetengo_lattice_entryView_valueOf(constrained_nodes[2].value_handle) ==
                    tetengo_lattice_entryView_valueOf(nodes[2].value_handle));
                BOOST_TEST(
                    tetengo_lattice_entryView_valueOf(constrained_nodes[3].value_handle) ==
                    tetengo_lattice_entryView_valueOf(nodes[3].value_handle));
            }
            tetengo_lattice_nBestIterator_next(p_constrained_iterator);
            {
                BOOST_TEST_REQUIRE(tetengo_lattice_nBestIterator_hasNext(p_constrained_iterator));
                BOOST_TEST_REQUIRE(tetengo_lattice_nBestIterator_hasNext(p_constrained_iterator));
                const auto* const p_constrained_path = tetengo_lattice_nBestIterator_createPath(p_constrained_iterator);
                BOOST_SCOPE_EXIT(p_constrained_path)
                {
                    tetengo_lattice_path_destroy(p_constrained_path);
                }
                BOOST_SCOPE_EXIT_END;
                BOOST_TEST_REQUIRE(p_constrained_path);

                const auto constrained_node_count = tetengo_lattice_path_pNodes(p_constrained_path, nullptr);
                BOOST_TEST_REQUIRE(constrained_node_count == 5U);
                std::vector<tetengo_lattice_node_t> constrained_nodes{ constrained_node_count };
                const auto                          constrained_node_count_again =
                    tetengo_lattice_path_pNodes(p_constrained_path, std::data(constrained_nodes));
                BOOST_TEST(constrained_node_count_again == 5U);
                BOOST_TEST(
                    tetengo_lattice_entryView_valueOf(constrained_nodes[0].value_handle) ==
                    tetengo_lattice_entryView_valueOf(nodes[0].value_handle));
                BOOST_TEST(
                    tetengo_lattice_entryView_valueOf(constrained_nodes[1].value_handle) ==
                    tetengo_lattice_entryView_valueOf(nodes[1].value_handle));
                const auto* const p_constrained_node_key2 =
                    tetengo_lattice_entryView_createKeyOf(constrained_nodes[2].key_handle);
                BOOST_SCOPE_EXIT(p_constrained_node_key2)
                {
                    tetengo_lattice_input_destroy(p_constrained_node_key2);
                }
                BOOST_SCOPE_EXIT_END;
                const auto constrained_node_key2 =
                    p_constrained_node_key2 ?
                        std::string_view{ tetengo_lattice_stringInput_value(p_constrained_node_key2) } :
                        std::string_view{};
                BOOST_TEST(constrained_node_key2 == "Tosu-Omuta");
                const auto* const p_constrained_node_key3 =
                    tetengo_lattice_entryView_createKeyOf(constrained_nodes[3].key_handle);
                BOOST_SCOPE_EXIT(p_constrained_node_key3)
                {
                    tetengo_lattice_input_destroy(p_constrained_node_key3);
                }
                BOOST_SCOPE_EXIT_END;
                const auto constrained_node_key3 =
                    p_constrained_node_key3 ?
                        std::string_view{ tetengo_lattice_stringInput_value(p_constrained_node_key3) } :
                        std::string_view{};
                BOOST_TEST(constrained_node_key3 == "Omuta-Kumamoto");
                BOOST_TEST(
                    tetengo_lattice_entryView_valueOf(constrained_nodes[4].value_handle) ==
                    tetengo_lattice_entryView_valueOf(nodes[3].value_handle));
            }
            tetengo_lattice_nBestIterator_next(p_constrained_iterator);
            BOOST_TEST(!tetengo_lattice_nBestIterator_hasNext(p_constrained_iterator));
        }
        {
            const auto* const p_path = tetengo_lattice_nBestIterator_createPath(p_iterator);
            BOOST_SCOPE_EXIT(p_path)
            {
                tetengo_lattice_path_destroy(p_path);
            }
            BOOST_SCOPE_EXIT_END;
            BOOST_TEST_REQUIRE(p_path);

            const auto node_count = tetengo_lattice_path_pNodes(p_path, nullptr);
            BOOST_TEST_REQUIRE(node_count == 4U);
            std::vector<tetengo_lattice_node_t> nodes{ node_count };
            const auto node_count_again = tetengo_lattice_path_pNodes(p_path, std::data(nodes));
            BOOST_TEST(node_count_again == 4U);

            std::vector<tetengo_lattice_constraintElement_t*> pattern{};
            pattern.reserve(4);
            pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&nodes[0]));
            pattern.push_back(tetengo_lattice_constraintElement_createWildcardConstraintElement(0));
            pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&nodes[2]));
            pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&nodes[3]));

            auto* const p_constrained_iterator = tetengo_lattice_nBestIterator_create(
                p_lattice, &eos_node, tetengo_lattice_constraint_create(std::data(pattern), std::size(pattern)));
            BOOST_SCOPE_EXIT(p_constrained_iterator)
            {
                tetengo_lattice_nBestIterator_destroy(p_constrained_iterator);
            }
            BOOST_SCOPE_EXIT_END;
            BOOST_TEST_REQUIRE(p_constrained_iterator);

            {
                BOOST_TEST_REQUIRE(tetengo_lattice_nBestIterator_hasNext(p_constrained_iterator));
                BOOST_TEST_REQUIRE(tetengo_lattice_nBestIterator_hasNext(p_constrained_iterator));
                const auto* const p_constrained_path = tetengo_lattice_nBestIterator_createPath(p_constrained_iterator);
                BOOST_SCOPE_EXIT(p_constrained_path)
                {
                    tetengo_lattice_path_destroy(p_constrained_path);
                }
                BOOST_SCOPE_EXIT_END;
                BOOST_TEST_REQUIRE(p_constrained_path);

                const auto constrained_node_count = tetengo_lattice_path_pNodes(p_constrained_path, nullptr);
                BOOST_TEST_REQUIRE(constrained_node_count == 4U);
                std::vector<tetengo_lattice_node_t> constrained_nodes{ constrained_node_count };
                const auto                          constrained_node_count_again =
                    tetengo_lattice_path_pNodes(p_constrained_path, std::data(constrained_nodes));
                BOOST_TEST(constrained_node_count_again == 4U);
                BOOST_TEST(
                    tetengo_lattice_entryView_valueOf(constrained_nodes[0].value_handle) ==
                    tetengo_lattice_entryView_valueOf(nodes[0].value_handle));
                BOOST_TEST(
                    *reinterpret_cast<const std::string*>(
                        tetengo_lattice_entryView_valueOf(constrained_nodes[1].value_handle)) == "local415");
                BOOST_TEST(
                    tetengo_lattice_entryView_valueOf(constrained_nodes[2].value_handle) ==
                    tetengo_lattice_entryView_valueOf(nodes[2].value_handle));
                BOOST_TEST(
                    tetengo_lattice_entryView_valueOf(constrained_nodes[3].value_handle) ==
                    tetengo_lattice_entryView_valueOf(nodes[3].value_handle));
            }
            tetengo_lattice_nBestIterator_next(p_constrained_iterator);
            {
                BOOST_TEST_REQUIRE(tetengo_lattice_nBestIterator_hasNext(p_constrained_iterator));
                BOOST_TEST_REQUIRE(tetengo_lattice_nBestIterator_hasNext(p_constrained_iterator));
                const auto* const p_constrained_path = tetengo_lattice_nBestIterator_createPath(p_constrained_iterator);
                BOOST_SCOPE_EXIT(p_constrained_path)
                {
                    tetengo_lattice_path_destroy(p_constrained_path);
                }
                BOOST_SCOPE_EXIT_END;
                BOOST_TEST_REQUIRE(p_constrained_path);

                const auto constrained_node_count = tetengo_lattice_path_pNodes(p_constrained_path, nullptr);
                BOOST_TEST_REQUIRE(constrained_node_count == 4U);
                std::vector<tetengo_lattice_node_t> constrained_nodes{ constrained_node_count };
                const auto                          constrained_node_count_again =
                    tetengo_lattice_path_pNodes(p_constrained_path, std::data(constrained_nodes));
                BOOST_TEST(constrained_node_count_again == 4U);
                BOOST_TEST(
                    tetengo_lattice_entryView_valueOf(constrained_nodes[0].value_handle) ==
                    tetengo_lattice_entryView_valueOf(nodes[0].value_handle));
                BOOST_TEST(
                    tetengo_lattice_entryView_valueOf(constrained_nodes[1].value_handle) ==
                    tetengo_lattice_entryView_valueOf(nodes[1].value_handle));
                BOOST_TEST(
                    tetengo_lattice_entryView_valueOf(constrained_nodes[2].value_handle) ==
                    tetengo_lattice_entryView_valueOf(nodes[2].value_handle));
                BOOST_TEST(
                    tetengo_lattice_entryView_valueOf(constrained_nodes[3].value_handle) ==
                    tetengo_lattice_entryView_valueOf(nodes[3].value_handle));
            }
            tetengo_lattice_nBestIterator_next(p_constrained_iterator);
            BOOST_TEST(!tetengo_lattice_nBestIterator_hasNext(p_constrained_iterator));
        }
        {
            const auto* const p_path = tetengo_lattice_nBestIterator_createPath(p_iterator);
            BOOST_SCOPE_EXIT(p_path)
            {
                tetengo_lattice_path_destroy(p_path);
            }
            BOOST_SCOPE_EXIT_END;
            BOOST_TEST_REQUIRE(p_path);

            const auto node_count = tetengo_lattice_path_pNodes(p_path, nullptr);
            BOOST_TEST_REQUIRE(node_count == 4U);
            std::vector<tetengo_lattice_node_t> nodes{ node_count };
            const auto node_count_again = tetengo_lattice_path_pNodes(p_path, std::data(nodes));
            BOOST_TEST(node_count_again == 4U);

            std::vector<tetengo_lattice_constraintElement_t*> pattern{};
            pattern.reserve(4);
            pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&nodes[0]));
            pattern.push_back(tetengo_lattice_constraintElement_createWildcardConstraintElement(0));
            pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&nodes[3]));

            auto* const p_constrained_iterator = tetengo_lattice_nBestIterator_create(
                p_lattice, &eos_node, tetengo_lattice_constraint_create(std::data(pattern), std::size(pattern)));
            BOOST_SCOPE_EXIT(p_constrained_iterator)
            {
                tetengo_lattice_nBestIterator_destroy(p_constrained_iterator);
            }
            BOOST_SCOPE_EXIT_END;
            BOOST_TEST_REQUIRE(p_constrained_iterator);

            auto path_count = static_cast<std::size_t>(0);
            while (tetengo_lattice_nBestIterator_hasNext(p_constrained_iterator))
            {
                tetengo_lattice_nBestIterator_next(p_constrained_iterator);
                ++path_count;
            }
            BOOST_TEST(path_count == 9U);
        }
    }
    {
        tetengo_lattice_nBestIterator_next(nullptr);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
