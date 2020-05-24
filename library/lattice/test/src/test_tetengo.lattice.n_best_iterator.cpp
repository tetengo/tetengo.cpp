/*! \file
    \brief An N-best lattice path iterator.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <any>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <boost/iterator/iterator_facade.hpp>
#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/connection.h> // IWYU pragma: keep
#include <tetengo/lattice/entry.h> // IWYU pragma: keep
#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/lattice.h>
#include <tetengo/lattice/lattice.hpp>
#include <tetengo/lattice/nBestIterator.h>
#include <tetengo/lattice/n_best_iterator.hpp>
#include <tetengo/lattice/node.h> // IWYU pragma: keep
#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/unordered_map_vocabulary.hpp>
#include <tetengo/lattice/vocabulary.h>
#include <tetengo/lattice/vocabulary.hpp>


namespace
{
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
              { "Hakata-Tosu-Omuta-Kumamoto", std::string{ "mizuho" }, 3670 },
              { "Hakata-Tosu-Omuta-Kumamoto", std::string{ "sakura" }, 2620 },
              { "Hakata-Tosu-Omuta-Kumamoto", std::string{ "tsubame" }, 2390 },
          } },
        { "[HakataTosu][TosuOmuta]",
          {
              { "Hakata-Tosu-Omuta", std::string{ "ariake" }, 2150 },
              { "Hakata-Tosu-Omuta", std::string{ "rapid811" }, 1310 },
          } },
        { "[HakataTosu]",
          {
              { "Hakata-Tosu", std::string{ "kamome" }, 840 },
              { "Hakata-Tosu", std::string{ "local415" }, 570 },
          } },
        { "[TosuOmuta]",
          {
              { "Tosu-Omuta", std::string{ "local813" }, 860 },
          } },
        { "[TosuOmuta][OmutaKumamoto]",
          {
              { "Tosu-Omuta-Kumamoto", std::string{ "local815" }, 1680 },
          } },
        { "[OmutaKumamoto]",
          {
              { "Omuta-Kumamoto", std::string{ "local817" }, 950 },
          } },
    };

    const std::vector<std::pair<std::pair<tetengo::lattice::entry, tetengo::lattice::entry>, int>> connections{
        { { tetengo::lattice::entry::bos_eos(), { "Hakata-Tosu-Omuta-Kumamoto", {}, 0 } }, 600 },
        { { tetengo::lattice::entry::bos_eos(), { "Hakata-Tosu-Omuta", {}, 0 } }, 700 },
        { { tetengo::lattice::entry::bos_eos(), { "Hakata-Tosu", {}, 0 } }, 800 },
        { { tetengo::lattice::entry::bos_eos(), tetengo::lattice::entry::bos_eos() }, 8000 },
        { { { "Hakata-Tosu", {}, 0 }, { "Tosu-Omuta-Kumamoto", {}, 0 } }, 500 },
        { { { "Hakata-Tosu", {}, 0 }, { "Tosu-Omuta", {}, 0 } }, 600 },
        { { { "Hakata-Tosu", {}, 0 }, tetengo::lattice::entry::bos_eos() }, 6000 },
        { { { "Hakata-Tosu-Omuta", {}, 0 }, { "Omuta-Kumamoto", {}, 0 } }, 200 },
        { { { "Hakata-Tosu-Omuta", {}, 0 }, tetengo::lattice::entry::bos_eos() }, 2000 },
        { { { "Tosu-Omuta", {}, 0 }, { "Omuta-Kumamoto", {}, 0 } }, 300 },
        { { { "Tosu-Omuta", {}, 0 }, tetengo::lattice::entry::bos_eos() }, 3000 },
        { { { "Hakata-Tosu-Omuta-Kumamoto", {}, 0 }, tetengo::lattice::entry::bos_eos() }, 400 },
        { { { "Tosu-Omuta-Kumamoto", {}, 0 }, tetengo::lattice::entry::bos_eos() }, 500 },
        { { { "Omuta-Kumamoto", {}, 0 }, tetengo::lattice::entry::bos_eos() }, 600 },
    };

    std::unique_ptr<tetengo::lattice::vocabulary> create_cpp_vocabulary()
    {
        return std::make_unique<tetengo::lattice::unordered_map_vocabulary>(entries, connections);
    }

    tetengo_lattice_vocabulary_t* create_c_vocabulary()
    {
        std::vector<tetengo_lattice_entry_t> entry_values{};
        std::vector<std::size_t>             entry_value_offsets{};
        entry_value_offsets.reserve(entries.size() + 1);
        for (const auto& e: entries)
        {
            entry_values.reserve(entry_values.size() + e.second.size());
            entry_value_offsets.push_back(entry_values.size());

            for (const auto& ev: e.second)
            {
                entry_values.push_back(
                    { { ev.key().c_str(), ev.key().length() }, std::any_cast<std::string>(&ev.value()), ev.cost() });
            }
        }
        entry_value_offsets.push_back(entry_values.size());

        std::vector<tetengo_lattice_keyEntriesPair_t> key_entries_pairs{};
        key_entries_pairs.reserve(entries.size());
        for (auto i = static_cast<std::size_t>(0); i < entries.size(); ++i)
        {
            const auto& entry = entries[i];
            key_entries_pairs.push_back({ { entry.first.c_str(), entry.first.length() },
                                          &entry_values[entry_value_offsets[i]],
                                          entry_value_offsets[i + 1] - entry_value_offsets[i] });
        }

        std::vector<tetengo_lattice_entry_t> connection_froms{};
        connection_froms.reserve(connections.size());
        std::vector<tetengo_lattice_entry_t> connection_tos{};
        connection_tos.reserve(connections.size());
        for (const auto& c: connections)
        {
            connection_froms.push_back({ { c.first.first.key().c_str(), c.first.first.key().length() },
                                         std::any_cast<std::string>(&c.first.first.value()),
                                         c.first.first.cost() });
            connection_tos.push_back({ { c.first.second.key().c_str(), c.first.second.key().length() },
                                       std::any_cast<std::string>(&c.first.second.value()),
                                       c.first.second.cost() });
        }

        std::vector<tetengo_lattice_entriesConnectionCostPair_t> entries_connection_cost_pairs{};
        entries_connection_cost_pairs.reserve(connections.size());
        for (auto i = static_cast<std::size_t>(0); i < connections.size(); ++i)
        {
            const auto& connection = connections[i];
            const auto& from = connection_froms[i];
            const auto& to = connection_tos[i];
            entries_connection_cost_pairs.push_back({ &from, &to, connection.second });
        }

        return tetengo_lattice_vocabulary_createUnorderedMapVocabulary(
            key_entries_pairs.data(),
            key_entries_pairs.size(),
            entries_connection_cost_pairs.data(),
            entries_connection_cost_pairs.size());
    }


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(lattice)
BOOST_AUTO_TEST_SUITE(cap)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    const std::vector<int>              preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
    const auto                          node = tetengo::lattice::node::eos(1, &preceding_edge_costs, 5, 42);
    std::vector<tetengo::lattice::node> nodes{ std::move(node) };
    const tetengo::lattice::cap         cap_{ std::move(nodes), 24, 42 };
}

BOOST_AUTO_TEST_CASE(operator_less)
{
    BOOST_TEST_PASSPOINT();

    const std::vector<int>              preceding_edge_costs1{ 3, 1, 4, 1, 5, 9, 2, 6 };
    const auto                          node1 = tetengo::lattice::node::eos(1, &preceding_edge_costs1, 5, 42);
    std::vector<tetengo::lattice::node> nodes1{ std::move(node1) };
    const tetengo::lattice::cap         cap1{ std::move(nodes1), 24, 42 };

    const std::vector<int>              preceding_edge_costs2{ 3, 1, 4, 1, 5, 9, 2, 6 };
    const auto                          node2 = tetengo::lattice::node::eos(1, &preceding_edge_costs2, 5, 42);
    std::vector<tetengo::lattice::node> nodes2{ std::move(node2) };
    const tetengo::lattice::cap         cap2{ std::move(nodes2), 24, 42 };

    const std::vector<int>              preceding_edge_costs3{ 2, 7, 1, 8, 2, 8 };
    const auto                          node3 = tetengo::lattice::node::eos(2, &preceding_edge_costs3, 3, 31);
    std::vector<tetengo::lattice::node> nodes3{ std::move(node3) };
    const tetengo::lattice::cap         cap3{ std::move(nodes3), 12, 4242 };

    BOOST_CHECK(!(cap1 < cap2));
    BOOST_CHECK(cap1 < cap3);
}

BOOST_AUTO_TEST_CASE(tail_path)
{
    BOOST_TEST_PASSPOINT();

    const std::vector<int>              preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
    const auto                          node = tetengo::lattice::node::eos(1, &preceding_edge_costs, 5, 42);
    std::vector<tetengo::lattice::node> nodes{ std::move(node) };
    const tetengo::lattice::cap         cap_{ std::move(nodes), 24, 42 };

    BOOST_TEST(cap_.tail_path().size() == 1U);
    BOOST_TEST(&cap_.tail_path()[0].preceding_edge_costs() == &preceding_edge_costs);
}

BOOST_AUTO_TEST_CASE(tail_path_cost)
{
    BOOST_TEST_PASSPOINT();

    const std::vector<int>              preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
    const auto                          node = tetengo::lattice::node::eos(1, &preceding_edge_costs, 5, 42);
    std::vector<tetengo::lattice::node> nodes{ std::move(node) };
    const tetengo::lattice::cap         cap_{ std::move(nodes), 24, 42 };

    BOOST_TEST(cap_.tail_path_cost() == 24);
}

BOOST_AUTO_TEST_CASE(whole_path_cost)
{
    BOOST_TEST_PASSPOINT();

    const std::vector<int>              preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
    const auto                          node = tetengo::lattice::node::eos(1, &preceding_edge_costs, 5, 42);
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
        tetengo::lattice::lattice lattice_{ create_cpp_vocabulary() };
        lattice_.push_back("[HakataTosu]");
        lattice_.push_back("[TosuOmuta]");
        lattice_.push_back("[OmutaKumamoto]");

        auto                                    eos_node_and_preceding_edge_costs = lattice_.settle();
        const tetengo::lattice::n_best_iterator iterator{ lattice_,
                                                          std::move(eos_node_and_preceding_edge_costs.first) };
    }

    {
        auto* const p_lattice = tetengo_lattice_lattice_create(create_c_vocabulary());
        BOOST_SCOPE_EXIT(p_lattice)
        {
            tetengo_lattice_lattice_destroy(p_lattice);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_lattice);
        tetengo_lattice_lattice_pushBack(p_lattice, "[HakataTosu]");
        tetengo_lattice_lattice_pushBack(p_lattice, "[TosuOmuta]");
        tetengo_lattice_lattice_pushBack(p_lattice, "[OmutaKumamoto]");

        const auto             preceding_edge_cost_count = tetengo_lattice_lattice_settle(p_lattice, nullptr, nullptr);
        tetengo_lattice_node_t eos_node{};
        std::vector<int>       preceding_edge_costs(preceding_edge_cost_count, 0);
        tetengo_lattice_lattice_settle(p_lattice, &eos_node, preceding_edge_costs.data());
        const auto* const p_iterator = tetengo_lattice_nBestIterator_create(p_lattice, &eos_node);
        BOOST_SCOPE_EXIT(p_iterator)
        {
            tetengo_lattice_nBestIterator_destroy(p_iterator);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_iterator);
    }
    {
        tetengo_lattice_node_t eos_node{};
        const auto* const      p_iterator = tetengo_lattice_nBestIterator_create(nullptr, &eos_node);
        BOOST_TEST(!p_iterator);
    }
    {
        auto* const p_lattice = tetengo_lattice_lattice_create(create_c_vocabulary());
        BOOST_SCOPE_EXIT(p_lattice)
        {
            tetengo_lattice_lattice_destroy(p_lattice);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_lattice);
        tetengo_lattice_lattice_pushBack(p_lattice, "[HakataTosu]");
        tetengo_lattice_lattice_pushBack(p_lattice, "[TosuOmuta]");
        tetengo_lattice_lattice_pushBack(p_lattice, "[OmutaKumamoto]");

        const auto* const p_iterator = tetengo_lattice_nBestIterator_create(p_lattice, nullptr);
        BOOST_TEST(!p_iterator);
    }
}

BOOST_AUTO_TEST_CASE(operator_dereference)
{
    BOOST_TEST_PASSPOINT();

    {
        tetengo::lattice::lattice lattice_{ create_cpp_vocabulary() };
        lattice_.push_back("[HakataTosu]");
        lattice_.push_back("[TosuOmuta]");
        lattice_.push_back("[OmutaKumamoto]");

        auto                                    eos_node_and_preceding_edge_costs = lattice_.settle();
        const tetengo::lattice::n_best_iterator iterator{ lattice_,
                                                          std::move(eos_node_and_preceding_edge_costs.first) };

        const auto& path = *iterator;
        BOOST_TEST_REQUIRE(path.size() == 3U);
        BOOST_TEST(!tetengo::lattice::temp::std_any_has_value(path[0].value()));
        BOOST_TEST(std::any_cast<std::string>(path[1].value()) == "tsubame");
        BOOST_TEST(!tetengo::lattice::temp::std_any_has_value(path[2].value()));
    }
    {
        const tetengo::lattice::n_best_iterator iterator{};

        BOOST_CHECK_THROW(*iterator, std::logic_error);
    }

    {
        auto* const p_lattice = tetengo_lattice_lattice_create(create_c_vocabulary());
        BOOST_SCOPE_EXIT(p_lattice)
        {
            tetengo_lattice_lattice_destroy(p_lattice);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_lattice);
        tetengo_lattice_lattice_pushBack(p_lattice, "[HakataTosu]");
        tetengo_lattice_lattice_pushBack(p_lattice, "[TosuOmuta]");
        tetengo_lattice_lattice_pushBack(p_lattice, "[OmutaKumamoto]");

        const auto             preceding_edge_cost_count = tetengo_lattice_lattice_settle(p_lattice, nullptr, nullptr);
        tetengo_lattice_node_t eos_node{};
        std::vector<int>       preceding_edge_costs(preceding_edge_cost_count, 0);
        tetengo_lattice_lattice_settle(p_lattice, &eos_node, preceding_edge_costs.data());
        const auto* const p_iterator = tetengo_lattice_nBestIterator_create(p_lattice, &eos_node);
        BOOST_SCOPE_EXIT(p_iterator)
        {
            tetengo_lattice_nBestIterator_destroy(p_iterator);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_iterator);

        const auto node_count = tetengo_lattice_nBestIterator_get(p_iterator, nullptr);
        BOOST_TEST_REQUIRE(node_count == 3U);

        std::vector<tetengo_lattice_node_t> nodes{ node_count };
        const auto node_count_again = tetengo_lattice_nBestIterator_get(p_iterator, nodes.data());
        BOOST_TEST(node_count_again == 3U);
        BOOST_TEST(!tetengo_lattice_entry_valueOf(nodes[0].value_handle));
        BOOST_TEST(
            *reinterpret_cast<const std::string*>(tetengo_lattice_entry_valueOf(nodes[1].value_handle)) == "tsubame");
        BOOST_TEST(!tetengo_lattice_entry_valueOf(nodes[2].value_handle));
    }
    {
        const auto node_count = tetengo_lattice_nBestIterator_get(nullptr, nullptr);
        BOOST_TEST(node_count == 0U);
    }
}

BOOST_AUTO_TEST_CASE(operator_equal)
{
    BOOST_TEST_PASSPOINT();

    {
        tetengo::lattice::lattice lattice_{ create_cpp_vocabulary() };
        lattice_.push_back("[HakataTosu]");
        lattice_.push_back("[TosuOmuta]");
        lattice_.push_back("[OmutaKumamoto]");

        auto                                    eos_node_and_preceding_edge_costs1 = lattice_.settle();
        const tetengo::lattice::n_best_iterator iterator1{ lattice_,
                                                           std::move(eos_node_and_preceding_edge_costs1.first) };
        auto                                    eos_node_and_preceding_edge_costs2 = lattice_.settle();
        const tetengo::lattice::n_best_iterator iterator2{ lattice_,
                                                           std::move(eos_node_and_preceding_edge_costs2.first) };
        auto                                    eos_node_and_preceding_edge_costs3 = lattice_.settle();
        tetengo::lattice::n_best_iterator iterator3{ lattice_, std::move(eos_node_and_preceding_edge_costs3.first) };
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
        tetengo::lattice::lattice lattice_{ create_cpp_vocabulary() };
        lattice_.push_back("[HakataTosu]");
        lattice_.push_back("[TosuOmuta]");

        auto                                    eos_node_and_preceding_edge_costs1 = lattice_.settle();
        const tetengo::lattice::n_best_iterator iterator1{ lattice_,
                                                           std::move(eos_node_and_preceding_edge_costs1.first) };

        lattice_.push_back("[OmutaKumamoto]");

        auto                                    eos_node_and_preceding_edge_costs2 = lattice_.settle();
        const tetengo::lattice::n_best_iterator iterator2{ lattice_,
                                                           std::move(eos_node_and_preceding_edge_costs2.first) };

        BOOST_CHECK(iterator1 != iterator2);
    }

    {
        auto* const p_lattice = tetengo_lattice_lattice_create(create_c_vocabulary());
        BOOST_SCOPE_EXIT(p_lattice)
        {
            tetengo_lattice_lattice_destroy(p_lattice);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_lattice);
        tetengo_lattice_lattice_pushBack(p_lattice, "[HakataTosu]");
        tetengo_lattice_lattice_pushBack(p_lattice, "[TosuOmuta]");
        tetengo_lattice_lattice_pushBack(p_lattice, "[OmutaKumamoto]");

        const auto             preceding_edge_cost_count = tetengo_lattice_lattice_settle(p_lattice, nullptr, nullptr);
        tetengo_lattice_node_t eos_node{};
        std::vector<int>       preceding_edge_costs(preceding_edge_cost_count, 0);
        tetengo_lattice_lattice_settle(p_lattice, &eos_node, preceding_edge_costs.data());
        auto* const p_iterator = tetengo_lattice_nBestIterator_create(p_lattice, &eos_node);
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
        tetengo::lattice::lattice lattice_{ create_cpp_vocabulary() };
        lattice_.push_back("[HakataTosu]");
        lattice_.push_back("[TosuOmuta]");
        lattice_.push_back("[OmutaKumamoto]");

        auto                              eos_node_and_preceding_edge_costs = lattice_.settle();
        tetengo::lattice::n_best_iterator iterator{ lattice_, std::move(eos_node_and_preceding_edge_costs.first) };
        {
            const auto& path = *iterator;
            BOOST_TEST_REQUIRE(path.size() == 3U);
            BOOST_TEST(!tetengo::lattice::temp::std_any_has_value(path[0].value()));
            BOOST_TEST(std::any_cast<std::string>(path[1].value()) == "tsubame");
            BOOST_TEST(!tetengo::lattice::temp::std_any_has_value(path[2].value()));
        }

        ++iterator;
        {
            const auto& path = *iterator;
            BOOST_TEST_REQUIRE(path.size() == 3U);
            BOOST_TEST(!tetengo::lattice::temp::std_any_has_value(path[0].value()));
            BOOST_TEST(std::any_cast<std::string>(path[1].value()) == "sakura");
            BOOST_TEST(!tetengo::lattice::temp::std_any_has_value(path[2].value()));
        }

        ++iterator;
        {
            const auto& path = *iterator;
            BOOST_TEST_REQUIRE(path.size() == 4U);
            BOOST_TEST(!tetengo::lattice::temp::std_any_has_value(path[0].value()));
            BOOST_TEST(std::any_cast<std::string>(path[1].value()) == "rapid811");
            BOOST_TEST(std::any_cast<std::string>(path[2].value()) == "local817");
            BOOST_TEST(!tetengo::lattice::temp::std_any_has_value(path[3].value()));
        }

        ++iterator;
        {
            const auto& path = *iterator;
            BOOST_TEST_REQUIRE(path.size() == 4U);
            BOOST_TEST(!tetengo::lattice::temp::std_any_has_value(path[0].value()));
            BOOST_TEST(std::any_cast<std::string>(path[1].value()) == "local415");
            BOOST_TEST(std::any_cast<std::string>(path[2].value()) == "local815");
            BOOST_TEST(!tetengo::lattice::temp::std_any_has_value(path[3].value()));
        }

        ++iterator;
        {
            const auto& path = *iterator;
            BOOST_TEST_REQUIRE(path.size() == 4U);
            BOOST_TEST(!tetengo::lattice::temp::std_any_has_value(path[0].value()));
            BOOST_TEST(std::any_cast<std::string>(path[1].value()) == "kamome");
            BOOST_TEST(std::any_cast<std::string>(path[2].value()) == "local815");
            BOOST_TEST(!tetengo::lattice::temp::std_any_has_value(path[3].value()));
        }

        ++iterator;
        {
            const auto& path = *iterator;
            BOOST_TEST_REQUIRE(path.size() == 4U);
            BOOST_TEST(!tetengo::lattice::temp::std_any_has_value(path[0].value()));
            BOOST_TEST(std::any_cast<std::string>(path[1].value()) == "ariake");
            BOOST_TEST(std::any_cast<std::string>(path[2].value()) == "local817");
            BOOST_TEST(!tetengo::lattice::temp::std_any_has_value(path[3].value()));
        }

        ++iterator;
        {
            const auto& path = *iterator;
            BOOST_TEST_REQUIRE(path.size() == 3U);
            BOOST_TEST(!tetengo::lattice::temp::std_any_has_value(path[0].value()));
            BOOST_TEST(std::any_cast<std::string>(path[1].value()) == "mizuho");
            BOOST_TEST(!tetengo::lattice::temp::std_any_has_value(path[2].value()));
        }

        ++iterator;
        {
            const auto& path = *iterator;
            BOOST_TEST_REQUIRE(path.size() == 5U);
            BOOST_TEST(!tetengo::lattice::temp::std_any_has_value(path[0].value()));
            BOOST_TEST(std::any_cast<std::string>(path[1].value()) == "local415");
            BOOST_TEST(std::any_cast<std::string>(path[2].value()) == "local813");
            BOOST_TEST(std::any_cast<std::string>(path[3].value()) == "local817");
            BOOST_TEST(!tetengo::lattice::temp::std_any_has_value(path[4].value()));
        }

        ++iterator;
        {
            const auto& path = *iterator;
            BOOST_TEST_REQUIRE(path.size() == 5U);
            BOOST_TEST(!tetengo::lattice::temp::std_any_has_value(path[0].value()));
            BOOST_TEST(std::any_cast<std::string>(path[1].value()) == "kamome");
            BOOST_TEST(std::any_cast<std::string>(path[2].value()) == "local813");
            BOOST_TEST(std::any_cast<std::string>(path[3].value()) == "local817");
            BOOST_TEST(!tetengo::lattice::temp::std_any_has_value(path[4].value()));
        }

        ++iterator;
        BOOST_CHECK_THROW(*iterator, std::logic_error);
    }

    {
        auto* const p_lattice = tetengo_lattice_lattice_create(create_c_vocabulary());
        BOOST_SCOPE_EXIT(p_lattice)
        {
            tetengo_lattice_lattice_destroy(p_lattice);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_lattice);
        tetengo_lattice_lattice_pushBack(p_lattice, "[HakataTosu]");
        tetengo_lattice_lattice_pushBack(p_lattice, "[TosuOmuta]");
        tetengo_lattice_lattice_pushBack(p_lattice, "[OmutaKumamoto]");

        const auto             preceding_edge_cost_count = tetengo_lattice_lattice_settle(p_lattice, nullptr, nullptr);
        tetengo_lattice_node_t eos_node{};
        std::vector<int>       preceding_edge_costs(preceding_edge_cost_count, 0);
        tetengo_lattice_lattice_settle(p_lattice, &eos_node, preceding_edge_costs.data());
        auto* const p_iterator = tetengo_lattice_nBestIterator_create(p_lattice, &eos_node);
        BOOST_SCOPE_EXIT(p_iterator)
        {
            tetengo_lattice_nBestIterator_destroy(p_iterator);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_iterator);

        {
            const auto node_count = tetengo_lattice_nBestIterator_get(p_iterator, nullptr);
            BOOST_TEST_REQUIRE(node_count == 3U);
            std::vector<tetengo_lattice_node_t> nodes{ node_count };
            const auto node_count_again = tetengo_lattice_nBestIterator_get(p_iterator, nodes.data());
            BOOST_TEST(node_count_again == 3U);
            BOOST_TEST(!tetengo_lattice_entry_valueOf(nodes[0].value_handle));
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entry_valueOf(nodes[1].value_handle)) ==
                "tsubame");
            BOOST_TEST(!tetengo_lattice_entry_valueOf(nodes[2].value_handle));
        }

        tetengo_lattice_nBestIterator_next(p_iterator);
        {
            const auto node_count = tetengo_lattice_nBestIterator_get(p_iterator, nullptr);
            BOOST_TEST_REQUIRE(node_count == 3U);
            std::vector<tetengo_lattice_node_t> nodes{ node_count };
            const auto node_count_again = tetengo_lattice_nBestIterator_get(p_iterator, nodes.data());
            BOOST_TEST(node_count_again == 3U);
            BOOST_TEST(!tetengo_lattice_entry_valueOf(nodes[0].value_handle));
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entry_valueOf(nodes[1].value_handle)) ==
                "sakura");
            BOOST_TEST(!tetengo_lattice_entry_valueOf(nodes[2].value_handle));
        }

        tetengo_lattice_nBestIterator_next(p_iterator);
        {
            const auto node_count = tetengo_lattice_nBestIterator_get(p_iterator, nullptr);
            BOOST_TEST_REQUIRE(node_count == 4U);
            std::vector<tetengo_lattice_node_t> nodes{ node_count };
            const auto node_count_again = tetengo_lattice_nBestIterator_get(p_iterator, nodes.data());
            BOOST_TEST(node_count_again == 4U);
            BOOST_TEST(!tetengo_lattice_entry_valueOf(nodes[0].value_handle));
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entry_valueOf(nodes[1].value_handle)) ==
                "rapid811");
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entry_valueOf(nodes[2].value_handle)) ==
                "local817");
            BOOST_TEST(!tetengo_lattice_entry_valueOf(nodes[3].value_handle));
        }

        tetengo_lattice_nBestIterator_next(p_iterator);
        {
            const auto node_count = tetengo_lattice_nBestIterator_get(p_iterator, nullptr);
            BOOST_TEST_REQUIRE(node_count == 4U);
            std::vector<tetengo_lattice_node_t> nodes{ node_count };
            const auto node_count_again = tetengo_lattice_nBestIterator_get(p_iterator, nodes.data());
            BOOST_TEST(node_count_again == 4U);
            BOOST_TEST(!tetengo_lattice_entry_valueOf(nodes[0].value_handle));
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entry_valueOf(nodes[1].value_handle)) ==
                "local415");
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entry_valueOf(nodes[2].value_handle)) ==
                "local815");
            BOOST_TEST(!tetengo_lattice_entry_valueOf(nodes[3].value_handle));
        }

        tetengo_lattice_nBestIterator_next(p_iterator);
        {
            const auto node_count = tetengo_lattice_nBestIterator_get(p_iterator, nullptr);
            BOOST_TEST_REQUIRE(node_count == 4U);
            std::vector<tetengo_lattice_node_t> nodes{ node_count };
            const auto node_count_again = tetengo_lattice_nBestIterator_get(p_iterator, nodes.data());
            BOOST_TEST(node_count_again == 4U);
            BOOST_TEST(!tetengo_lattice_entry_valueOf(nodes[0].value_handle));
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entry_valueOf(nodes[1].value_handle)) ==
                "kamome");
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entry_valueOf(nodes[2].value_handle)) ==
                "local815");
            BOOST_TEST(!tetengo_lattice_entry_valueOf(nodes[3].value_handle));
        }

        tetengo_lattice_nBestIterator_next(p_iterator);
        {
            const auto node_count = tetengo_lattice_nBestIterator_get(p_iterator, nullptr);
            BOOST_TEST_REQUIRE(node_count == 4U);
            std::vector<tetengo_lattice_node_t> nodes{ node_count };
            const auto node_count_again = tetengo_lattice_nBestIterator_get(p_iterator, nodes.data());
            BOOST_TEST(node_count_again == 4U);
            BOOST_TEST(!tetengo_lattice_entry_valueOf(nodes[0].value_handle));
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entry_valueOf(nodes[1].value_handle)) ==
                "ariake");
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entry_valueOf(nodes[2].value_handle)) ==
                "local817");
            BOOST_TEST(!tetengo_lattice_entry_valueOf(nodes[3].value_handle));
        }

        tetengo_lattice_nBestIterator_next(p_iterator);
        {
            const auto node_count = tetengo_lattice_nBestIterator_get(p_iterator, nullptr);
            BOOST_TEST_REQUIRE(node_count == 3U);
            std::vector<tetengo_lattice_node_t> nodes{ node_count };
            const auto node_count_again = tetengo_lattice_nBestIterator_get(p_iterator, nodes.data());
            BOOST_TEST(node_count_again == 3U);
            BOOST_TEST(!tetengo_lattice_entry_valueOf(nodes[0].value_handle));
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entry_valueOf(nodes[1].value_handle)) ==
                "mizuho");
            BOOST_TEST(!tetengo_lattice_entry_valueOf(nodes[2].value_handle));
        }

        tetengo_lattice_nBestIterator_next(p_iterator);
        {
            const auto node_count = tetengo_lattice_nBestIterator_get(p_iterator, nullptr);
            BOOST_TEST_REQUIRE(node_count == 5U);
            std::vector<tetengo_lattice_node_t> nodes{ node_count };
            const auto node_count_again = tetengo_lattice_nBestIterator_get(p_iterator, nodes.data());
            BOOST_TEST(node_count_again == 5U);
            BOOST_TEST(!tetengo_lattice_entry_valueOf(nodes[0].value_handle));
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entry_valueOf(nodes[1].value_handle)) ==
                "local415");
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entry_valueOf(nodes[2].value_handle)) ==
                "local813");
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entry_valueOf(nodes[3].value_handle)) ==
                "local817");
            BOOST_TEST(!tetengo_lattice_entry_valueOf(nodes[4].value_handle));
        }

        tetengo_lattice_nBestIterator_next(p_iterator);
        {
            const auto node_count = tetengo_lattice_nBestIterator_get(p_iterator, nullptr);
            BOOST_TEST_REQUIRE(node_count == 5U);
            std::vector<tetengo_lattice_node_t> nodes{ node_count };
            const auto node_count_again = tetengo_lattice_nBestIterator_get(p_iterator, nodes.data());
            BOOST_TEST(node_count_again == 5U);
            BOOST_TEST(!tetengo_lattice_entry_valueOf(nodes[0].value_handle));
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entry_valueOf(nodes[1].value_handle)) ==
                "kamome");
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entry_valueOf(nodes[2].value_handle)) ==
                "local813");
            BOOST_TEST(
                *reinterpret_cast<const std::string*>(tetengo_lattice_entry_valueOf(nodes[3].value_handle)) ==
                "local817");
            BOOST_TEST(!tetengo_lattice_entry_valueOf(nodes[4].value_handle));
        }

        tetengo_lattice_nBestIterator_next(p_iterator);
        {
            const auto node_count = tetengo_lattice_nBestIterator_get(p_iterator, nullptr);
            BOOST_TEST(node_count == 0U);
        }
    }
    {
        tetengo_lattice_nBestIterator_next(nullptr);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
