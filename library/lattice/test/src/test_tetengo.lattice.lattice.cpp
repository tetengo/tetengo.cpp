/*! \file
    \brief A lattice.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <any>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/connection.h> // IWYU pragma: keep
#include <tetengo/lattice/entry.h> // IWYU pragma: keep
#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/lattice.h>
#include <tetengo/lattice/lattice.hpp>
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
        (1) 3390  BOS - tsubame - EOS
        (2) 3620  BOS - sakura - EOS
        (3) 3760  BOS - rapid811 - local817 - EOS
        (4) 4050  BOS - local415 - local815 - EOS
        (5) 4320  BOS - kamome - local815 - EOS
        (6) 4600  BOS - ariake - local817 - EOS
        (7) 4670  BOS - mizuho - EOS
        (8) 4680  BOS - local415 - local813 - local817 - EOS
        (9) 4950  BOS - kamome - local813 - local817 - EOS
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
                entry_values.push_back({ { ev.key().c_str(), ev.key().length() }, &ev.value(), ev.cost() });
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
                                         &c.first.first.value(),
                                         c.first.first.cost() });
            connection_tos.push_back({ { c.first.second.key().c_str(), c.first.second.key().length() },
                                       &c.first.second.value(),
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
BOOST_AUTO_TEST_SUITE(lattice)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::lattice lattice_{ create_cpp_vocabulary() };
    }

    {
        const auto* const p_lattice = tetengo_lattice_lattice_create(create_c_vocabulary());
        BOOST_SCOPE_EXIT(p_lattice)
        {
            tetengo_lattice_lattice_destroy(p_lattice);
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
        tetengo::lattice::lattice lattice_{ create_cpp_vocabulary() };

        BOOST_TEST(lattice_.step_count() == 1U);

        lattice_.push_back("[HakataTosu]");

        BOOST_TEST(lattice_.step_count() == 2U);

        lattice_.push_back("[TosuOmuta]");

        BOOST_TEST(lattice_.step_count() == 3U);

        lattice_.push_back("[OmutaKumamoto]");

        BOOST_TEST(lattice_.step_count() == 4U);
    }

    {
        auto* const p_lattice = tetengo_lattice_lattice_create(create_c_vocabulary());
        BOOST_SCOPE_EXIT(p_lattice)
        {
            tetengo_lattice_lattice_destroy(p_lattice);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_lattice);

        BOOST_TEST(tetengo_lattice_lattice_stepCount(p_lattice) == 1U);

        tetengo_lattice_lattice_pushBack(p_lattice, "[HakataTosu]");

        BOOST_TEST(tetengo_lattice_lattice_stepCount(p_lattice) == 2U);

        tetengo_lattice_lattice_pushBack(p_lattice, "[TosuOmuta]");

        BOOST_TEST(tetengo_lattice_lattice_stepCount(p_lattice) == 3U);

        tetengo_lattice_lattice_pushBack(p_lattice, "[OmutaKumamoto]");

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
        tetengo::lattice::lattice lattice_{ create_cpp_vocabulary() };
        lattice_.push_back("[HakataTosu]");
        lattice_.push_back("[TosuOmuta]");
        lattice_.push_back("[OmutaKumamoto]");

        {
            const auto& nodes = lattice_.nodes_at(0);

            BOOST_TEST_REQUIRE(nodes.size() == 1U);
            BOOST_TEST(nodes[0].value().has_value() == tetengo::lattice::node::bos().value().has_value());
        }
        {
            const auto& nodes = lattice_.nodes_at(1);

            BOOST_TEST_REQUIRE(nodes.size() == 2U);
            BOOST_TEST(std::any_cast<std::string>(nodes[0].value()) == "kamome");
            BOOST_TEST(std::any_cast<std::string>(nodes[1].value()) == "local415");
        }
        {
            const auto& nodes = lattice_.nodes_at(2);

            BOOST_TEST_REQUIRE(nodes.size() == 3U);
            BOOST_TEST(std::any_cast<std::string>(nodes[0].value()) == "ariake");
            BOOST_TEST(std::any_cast<std::string>(nodes[1].value()) == "rapid811");
            BOOST_TEST(std::any_cast<std::string>(nodes[2].value()) == "local813");
        }
        {
            const auto& nodes = lattice_.nodes_at(3);

            BOOST_TEST_REQUIRE(nodes.size() == 5U);
            BOOST_TEST(std::any_cast<std::string>(nodes[0].value()) == "mizuho");
            BOOST_TEST(std::any_cast<std::string>(nodes[1].value()) == "sakura");
            BOOST_TEST(std::any_cast<std::string>(nodes[2].value()) == "tsubame");
            BOOST_TEST(std::any_cast<std::string>(nodes[3].value()) == "local815");
            BOOST_TEST(std::any_cast<std::string>(nodes[4].value()) == "local817");
        }
        {
            BOOST_CHECK_THROW(lattice_.nodes_at(4), std::out_of_range);
        }
    }

#if 0
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

        {
            const auto node_count = tetengo_lattice_lattice_nodesAt(p_lattice, 0, nullptr);
            BOOST_TEST_REQUIRE(node_count == 1U);

            std::vector<tetengo_lattice_node_t> nodes{ node_count };
            const auto node_count_again = tetengo_lattice_lattice_nodesAt(p_lattice, 0, nodes.data());
            BOOST_TEST(node_count_again == 1U);

            BOOST_TEST(nodes[0].p_value == tetengo_lattice_node_bos()->p_value);
        }
        {
            const auto node_count = tetengo_lattice_lattice_nodesAt(p_lattice, 1, nullptr);
            BOOST_TEST_REQUIRE(node_count == 2U);

            std::vector<tetengo_lattice_node_t> nodes{ node_count };
            const auto node_count_again = tetengo_lattice_lattice_nodesAt(p_lattice, 1, nodes.data());
            BOOST_TEST(node_count_again == 2U);

            BOOST_TEST(*reinterpret_cast<const std::string*>(nodes[0].p_value) == "kamome");
            BOOST_TEST(*reinterpret_cast<const std::string*>(nodes[1].p_value) == "local415");
        }
        {
            const auto node_count = tetengo_lattice_lattice_nodesAt(p_lattice, 2, nullptr);
            BOOST_TEST_REQUIRE(node_count == 3U);

            std::vector<tetengo_lattice_node_t> nodes{ node_count };
            const auto node_count_again = tetengo_lattice_lattice_nodesAt(p_lattice, 2, nodes.data());
            BOOST_TEST(node_count_again == 3U);

            BOOST_TEST(*reinterpret_cast<const std::string*>(nodes[0].p_value) == "ariake");
            BOOST_TEST(*reinterpret_cast<const std::string*>(nodes[1].p_value) == "rapid811");
            BOOST_TEST(*reinterpret_cast<const std::string*>(nodes[2].p_value) == "local813");
        }
        {
            const auto node_count = tetengo_lattice_lattice_nodesAt(p_lattice, 3, nullptr);
            BOOST_TEST_REQUIRE(node_count == 5U);

            std::vector<tetengo_lattice_node_t> nodes{ node_count };
            const auto node_count_again = tetengo_lattice_lattice_nodesAt(p_lattice, 3, nodes.data());
            BOOST_TEST(node_count_again == 5U);

            BOOST_TEST(*reinterpret_cast<const std::string*>(nodes[0].p_value) == "mizuho");
            BOOST_TEST(*reinterpret_cast<const std::string*>(nodes[1].p_value) == "sakura");
            BOOST_TEST(*reinterpret_cast<const std::string*>(nodes[2].p_value) == "tsubame");
            BOOST_TEST(*reinterpret_cast<const std::string*>(nodes[3].p_value) == "local815");
            BOOST_TEST(*reinterpret_cast<const std::string*>(nodes[4].p_value) == "local817");
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
#endif
}

BOOST_AUTO_TEST_CASE(push_back)
{
    BOOST_TEST_PASSPOINT();

    {
        tetengo::lattice::lattice lattice_{ create_cpp_vocabulary() };

        lattice_.push_back("[HakataTosu]");
        lattice_.push_back("[TosuOmuta]");
        lattice_.push_back("[OmutaKumamoto]");
    }

    {
        auto* const p_lattice = tetengo_lattice_lattice_create(create_c_vocabulary());
        BOOST_SCOPE_EXIT(p_lattice)
        {
            tetengo_lattice_lattice_destroy(p_lattice);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_lattice);

        BOOST_TEST(tetengo_lattice_lattice_pushBack(p_lattice, "[HakataTosu]"));
        BOOST_TEST(tetengo_lattice_lattice_pushBack(p_lattice, "[TosuOmuta]"));
        BOOST_TEST(tetengo_lattice_lattice_pushBack(p_lattice, "[OmutaKumamoto]"));
    }
    {
        auto* const p_lattice = tetengo_lattice_lattice_create(create_c_vocabulary());
        BOOST_SCOPE_EXIT(p_lattice)
        {
            tetengo_lattice_lattice_destroy(p_lattice);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_lattice);

        BOOST_TEST(!tetengo_lattice_lattice_pushBack(nullptr, "[HakataTosu]"));
        BOOST_TEST(!tetengo_lattice_lattice_pushBack(p_lattice, nullptr));
    }
}

BOOST_AUTO_TEST_CASE(settle)
{
    BOOST_TEST_PASSPOINT();

    {
        tetengo::lattice::lattice lattice_{ create_cpp_vocabulary() };

        {
            const auto eos_node = lattice_.settle();

            BOOST_TEST(eos_node.preceding_step() == 0U);
            BOOST_TEST(eos_node.best_preceding_node() == 0U);
            BOOST_TEST(eos_node.path_cost() == 8000);
        }

        lattice_.push_back("[HakataTosu]");
        {
            const auto eos_node = lattice_.settle();

            BOOST_TEST(eos_node.preceding_step() == 1U);
            BOOST_TEST(eos_node.best_preceding_node() == 1U);
            BOOST_TEST(eos_node.path_cost() == 7370);
        }

        lattice_.push_back("[TosuOmuta]");
        {
            const auto eos_node = lattice_.settle();

            BOOST_TEST(eos_node.preceding_step() == 2U);
            BOOST_TEST(eos_node.best_preceding_node() == 1U);
            BOOST_TEST(eos_node.path_cost() == 4010);
        }

        lattice_.push_back("[OmutaKumamoto]");
        {
            const auto eos_node = lattice_.settle();

            BOOST_TEST(eos_node.preceding_step() == 3U);
            BOOST_TEST(eos_node.best_preceding_node() == 2U);
            BOOST_TEST(eos_node.path_cost() == 3390);
        }
    }

    {
        auto* const p_lattice = tetengo_lattice_lattice_create(create_c_vocabulary());
        BOOST_SCOPE_EXIT(p_lattice)
        {
            tetengo_lattice_lattice_destroy(p_lattice);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_lattice);

        {
            tetengo_lattice_node_t eos_node{};
            BOOST_TEST(tetengo_lattice_lattice_settle(p_lattice, &eos_node));

            BOOST_TEST(eos_node.preceding_step == 0U);
            BOOST_TEST(eos_node.best_preceding_node == 0U);
            BOOST_TEST(eos_node.path_cost == 8000);
        }

        BOOST_TEST(tetengo_lattice_lattice_pushBack(p_lattice, "[HakataTosu]"));
        {
            tetengo_lattice_node_t eos_node{};
            BOOST_TEST(tetengo_lattice_lattice_settle(p_lattice, &eos_node));

            BOOST_TEST(eos_node.preceding_step == 1U);
            BOOST_TEST(eos_node.best_preceding_node == 1U);
            BOOST_TEST(eos_node.path_cost == 7370);
        }

        BOOST_TEST(tetengo_lattice_lattice_pushBack(p_lattice, "[TosuOmuta]"));
        {
            tetengo_lattice_node_t eos_node{};
            BOOST_TEST(tetengo_lattice_lattice_settle(p_lattice, &eos_node));

            BOOST_TEST(eos_node.preceding_step == 2U);
            BOOST_TEST(eos_node.best_preceding_node == 1U);
            BOOST_TEST(eos_node.path_cost == 4010);
        }

        BOOST_TEST(tetengo_lattice_lattice_pushBack(p_lattice, "[OmutaKumamoto]"));
        {
            tetengo_lattice_node_t eos_node{};
            BOOST_TEST(tetengo_lattice_lattice_settle(p_lattice, &eos_node));

            BOOST_TEST(eos_node.preceding_step == 3U);
            BOOST_TEST(eos_node.best_preceding_node == 2U);
            BOOST_TEST(eos_node.path_cost == 3390);
        }
    }
    {
        auto* const p_lattice = tetengo_lattice_lattice_create(create_c_vocabulary());
        BOOST_SCOPE_EXIT(p_lattice)
        {
            tetengo_lattice_lattice_destroy(p_lattice);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_lattice);

        tetengo_lattice_node_t eos_node{};
        BOOST_TEST(!tetengo_lattice_lattice_settle(nullptr, &eos_node));
        BOOST_TEST(!tetengo_lattice_lattice_settle(p_lattice, nullptr));
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
