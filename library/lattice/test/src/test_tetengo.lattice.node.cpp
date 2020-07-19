/*! \file
    \brief A node.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <any>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <string_view>
#include <vector>

#include <boost/core/ignore_unused.hpp>
#include <boost/operators.hpp>
#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/entry.h> // IWYU pragma: keep
#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/node.h>
#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/stringView.h>


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(lattice)
BOOST_AUTO_TEST_SUITE(node)


BOOST_AUTO_TEST_CASE(bos)
{
    BOOST_TEST_PASSPOINT();

    {
        const std::vector<int> preceding_edge_costs{};
        const auto             bos = tetengo::lattice::node::bos(&preceding_edge_costs);

        BOOST_TEST(bos.key() == tetengo::lattice::entry_view::bos_eos().key());
        BOOST_TEST(!tetengo::lattice::temp::std_any_has_value(bos.value()));
        BOOST_TEST(bos.preceding_step() == std::numeric_limits<std::size_t>::max());
        BOOST_TEST(&bos.preceding_edge_costs() == &preceding_edge_costs);
        BOOST_TEST(bos.best_preceding_node() == std::numeric_limits<std::size_t>::max());
        BOOST_TEST(bos.node_cost() == tetengo::lattice::entry_view::bos_eos().cost());
        BOOST_TEST(bos.path_cost() == 0);
    }
    {
        BOOST_CHECK_THROW(tetengo::lattice::node::bos(nullptr), std::invalid_argument);
    }

    {
        const std::vector<int> preceding_edge_costs{};
        tetengo_lattice_node_t bos{};
        const auto result = tetengo_lattice_node_bos(preceding_edge_costs.data(), preceding_edge_costs.size(), &bos);
        BOOST_TEST(result);

        BOOST_TEST(bos.key.p_head == tetengo_lattice_entry_bosEos()->key.p_head);
        BOOST_TEST(bos.key.length == tetengo_lattice_entry_bosEos()->key.length);
        BOOST_TEST(!tetengo_lattice_entry_valueOf(bos.value_handle));
        BOOST_TEST(bos.preceding_step == std::numeric_limits<std::size_t>::max());
        BOOST_TEST(bos.p_preceding_edge_costs == preceding_edge_costs.data());
        BOOST_TEST(bos.preceding_edge_cost_count == preceding_edge_costs.size());
        BOOST_TEST(bos.best_preceding_node == std::numeric_limits<std::size_t>::max());
        BOOST_TEST(bos.node_cost == tetengo_lattice_entry_bosEos()->cost);
        BOOST_TEST(bos.path_cost == 0);
    }
    {
        tetengo_lattice_node_t bos{};
        const auto             result = tetengo_lattice_node_bos(nullptr, 0, &bos);
        BOOST_TEST_REQUIRE(result);

        BOOST_TEST(bos.key.p_head == tetengo_lattice_entry_bosEos()->key.p_head);
        BOOST_TEST(bos.key.length == tetengo_lattice_entry_bosEos()->key.length);
        BOOST_TEST(!tetengo_lattice_entry_valueOf(bos.value_handle));
        BOOST_TEST(bos.preceding_step == std::numeric_limits<std::size_t>::max());
        BOOST_TEST(bos.preceding_edge_cost_count == 0U);
        BOOST_TEST(bos.best_preceding_node == std::numeric_limits<std::size_t>::max());
        BOOST_TEST(bos.node_cost == tetengo_lattice_entry_bosEos()->cost);
        BOOST_TEST(bos.path_cost == 0);
    }
    {
        tetengo_lattice_node_t bos{};
        const auto             result = tetengo_lattice_node_bos(nullptr, 3, &bos);
        BOOST_TEST(!result);
    }
    {
        const std::vector<int> preceding_edge_costs{};
        const auto result = tetengo_lattice_node_bos(preceding_edge_costs.data(), preceding_edge_costs.size(), nullptr);
        BOOST_TEST(!result);
    }
}

BOOST_AUTO_TEST_CASE(eos)
{
    BOOST_TEST_PASSPOINT();

    {
        const std::vector<int> preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
        const auto             eos = tetengo::lattice::node::eos(1, &preceding_edge_costs, 5, 42);

        BOOST_TEST(eos.key() == tetengo::lattice::entry_view::bos_eos().key());
        BOOST_TEST(!tetengo::lattice::temp::std_any_has_value(eos.value()));
        BOOST_TEST(eos.preceding_step() == 1U);
        BOOST_TEST(&eos.preceding_edge_costs() == &preceding_edge_costs);
        BOOST_TEST(eos.best_preceding_node() == 5U);
        BOOST_TEST(eos.node_cost() == tetengo::lattice::entry_view::bos_eos().cost());
        BOOST_TEST(eos.path_cost() == 42);
    }
    {
        BOOST_CHECK_THROW(tetengo::lattice::node::eos(1, nullptr, 5, 42), std::invalid_argument);
    }


    {
        const std::vector<int> preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
        tetengo_lattice_node_t eos{};
        const auto             result =
            tetengo_lattice_node_eos(1, preceding_edge_costs.data(), preceding_edge_costs.size(), 5, 42, &eos);
        BOOST_TEST(result);

        BOOST_TEST(eos.key.p_head == tetengo_lattice_entry_bosEos()->key.p_head);
        BOOST_TEST(eos.key.length == tetengo_lattice_entry_bosEos()->key.length);
        BOOST_TEST(!tetengo_lattice_entry_valueOf(eos.value_handle));
        BOOST_TEST(eos.preceding_step == 1U);
        BOOST_TEST(eos.p_preceding_edge_costs == preceding_edge_costs.data());
        BOOST_TEST(eos.preceding_edge_cost_count == preceding_edge_costs.size());
        BOOST_TEST(eos.best_preceding_node == 5U);
        BOOST_TEST(eos.node_cost == tetengo_lattice_entry_bosEos()->cost);
        BOOST_TEST(eos.path_cost == 42);
    }
    {
        tetengo_lattice_node_t eos{};
        const auto             result = tetengo_lattice_node_eos(1, nullptr, 0, 5, 42, &eos);
        BOOST_TEST_REQUIRE(result);

        BOOST_TEST(eos.key.p_head == tetengo_lattice_entry_bosEos()->key.p_head);
        BOOST_TEST(eos.key.length == tetengo_lattice_entry_bosEos()->key.length);
        BOOST_TEST(!tetengo_lattice_entry_valueOf(eos.value_handle));
        BOOST_TEST(eos.preceding_step == 1U);
        BOOST_TEST(eos.preceding_edge_cost_count == 0U);
        BOOST_TEST(eos.best_preceding_node == 5U);
        BOOST_TEST(eos.node_cost == tetengo_lattice_entry_bosEos()->cost);
        BOOST_TEST(eos.path_cost == 42);
    }
    {
        tetengo_lattice_node_t eos{};
        const auto             result = tetengo_lattice_node_eos(1, nullptr, 3, 5, 42, &eos);
        BOOST_TEST(!result);
    }
    {
        const std::vector<int> preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
        const auto             result =
            tetengo_lattice_node_eos(1, preceding_edge_costs.data(), preceding_edge_costs.size(), 5, 42, nullptr);
        BOOST_TEST(!result);
    }
}

BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const std::any               value{ 42 };
        const std::vector<int>       preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
        const tetengo::lattice::node node_{ "mizuho", &value, 1, &preceding_edge_costs, 5, 24, 2424 };
    }
    {
        const std::any                     entry_value{ 42 };
        const tetengo::lattice::entry_view entry{ "mizuho", &entry_value, 24 };
        const std::vector<int>             preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
        const tetengo::lattice::node       node_{ entry, 1, &preceding_edge_costs, 5, 2424 };

        BOOST_TEST(node_.key() == "mizuho");
        BOOST_TEST(std::any_cast<int>(node_.value()) == 42);
        BOOST_TEST(node_.preceding_step() == 1U);
        BOOST_TEST(&node_.preceding_edge_costs() == &preceding_edge_costs);
        BOOST_TEST(node_.best_preceding_node() == 5U);
        BOOST_TEST(node_.node_cost() == 24);
        BOOST_TEST(node_.path_cost() == 2424);
    }
    {
        const std::vector<int> preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
        BOOST_CHECK_THROW(
            const tetengo::lattice::node node_("mizuho", nullptr, 1, &preceding_edge_costs, 5, 24, 2424),
            std::invalid_argument);
    }
    {
        const std::any value{ 42 };
        BOOST_CHECK_THROW(
            const tetengo::lattice::node node_("mizuho", &value, 1, nullptr, 5, 24, 2424), std::invalid_argument);
    }
    {
        const std::any                     entry_value{ 42 };
        const tetengo::lattice::entry_view entry{ "mizuho", &entry_value, 24 };
        BOOST_CHECK_THROW(const tetengo::lattice::node node_(entry, 1, nullptr, 5, 2424), std::invalid_argument);
    }

    {
        const std::string_view       key{ "mizuho" };
        const std::any               value{ reinterpret_cast<const void*>("MIZUHO") };
        const std::vector<int>       preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
        const tetengo_lattice_node_t node{ { key.data(), key.length() },
                                           reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&value),
                                           1,
                                           preceding_edge_costs.data(),
                                           preceding_edge_costs.size(),
                                           5,
                                           24,
                                           2424 };
        boost::ignore_unused(node);
    }
    {
        const std::string_view            key{ "mizuho" };
        const std::any                    value{ reinterpret_cast<const void*>("MIZUHO") };
        const tetengo_lattice_entryView_t entry{ { key.data(), key.length() },
                                                 reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&value),
                                                 24 };
        const std::vector<int>            preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
        tetengo_lattice_node_t            node{};
        const auto                        result = tetengo_lattice_node_toNode(
            &entry, 1, preceding_edge_costs.data(), preceding_edge_costs.size(), 5, 2424, &node);
        BOOST_TEST_REQUIRE(result);

        BOOST_TEST(node.key.p_head == entry.key.p_head);
        BOOST_TEST(node.key.length == entry.key.length);
        BOOST_TEST(node.value_handle == entry.value_handle);
        BOOST_TEST(node.preceding_step == 1U);
        BOOST_TEST(node.p_preceding_edge_costs == preceding_edge_costs.data());
        BOOST_TEST(node.preceding_edge_cost_count == preceding_edge_costs.size());
        BOOST_TEST(node.best_preceding_node == 5U);
        BOOST_TEST(node.node_cost == entry.cost);
        BOOST_TEST(node.path_cost == 2424);
    }
    {
        const std::string_view            key{ "mizuho" };
        const std::any                    value{ static_cast<const void*>(nullptr) };
        const tetengo_lattice_entryView_t entry{ { key.data(), key.length() },
                                                 reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&value),
                                                 24 };
        const std::vector<int>            preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
        tetengo_lattice_node_t            node{};
        const auto                        result = tetengo_lattice_node_toNode(
            &entry, 1, preceding_edge_costs.data(), preceding_edge_costs.size(), 5, 2424, &node);
        BOOST_TEST_REQUIRE(result);

        BOOST_TEST(node.key.p_head == entry.key.p_head);
        BOOST_TEST(node.key.length == entry.key.length);
        BOOST_TEST(!tetengo_lattice_entry_valueOf(node.value_handle));
        BOOST_TEST(node.preceding_step == 1U);
        BOOST_TEST(node.p_preceding_edge_costs == preceding_edge_costs.data());
        BOOST_TEST(node.preceding_edge_cost_count == preceding_edge_costs.size());
        BOOST_TEST(node.best_preceding_node == 5U);
        BOOST_TEST(node.node_cost == entry.cost);
        BOOST_TEST(node.path_cost == 2424);
    }
    {
        const std::vector<int> preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
        tetengo_lattice_node_t node{};
        const auto             result = tetengo_lattice_node_toNode(
            nullptr, 1, preceding_edge_costs.data(), preceding_edge_costs.size(), 5, 2424, &node);
        BOOST_TEST(!result);
    }
    {
        const std::string_view            key{ "mizuho" };
        const std::any                    value{ reinterpret_cast<const void*>("MIZUHO") };
        const tetengo_lattice_entryView_t entry{ { key.data(), key.length() },
                                                 reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&value),
                                                 24 };
        tetengo_lattice_node_t            node{};
        const auto                        result = tetengo_lattice_node_toNode(&entry, 1, nullptr, 0, 5, 2424, &node);
        BOOST_TEST_REQUIRE(result);

        BOOST_TEST(node.key.p_head == entry.key.p_head);
        BOOST_TEST(node.key.length == entry.key.length);
        BOOST_TEST(node.value_handle == entry.value_handle);
        BOOST_TEST(node.preceding_step == 1U);
        BOOST_TEST(node.preceding_edge_cost_count == 0U);
        BOOST_TEST(node.best_preceding_node == 5U);
        BOOST_TEST(node.node_cost == entry.cost);
        BOOST_TEST(node.path_cost == 2424);
    }
    {
        const std::string_view            key{ "mizuho" };
        const std::any                    value{ reinterpret_cast<const void*>("MIZUHO") };
        const tetengo_lattice_entryView_t entry{ { key.data(), key.length() },
                                                 reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&value),
                                                 24 };
        tetengo_lattice_node_t            node{};
        const auto                        result = tetengo_lattice_node_toNode(&entry, 1, nullptr, 3, 5, 2424, &node);
        BOOST_TEST(!result);
    }
    {
        const std::string_view            key{ "mizuho" };
        const std::any                    value{ reinterpret_cast<const void*>("MIZUHO") };
        const tetengo_lattice_entryView_t entry{ { key.data(), key.length() },
                                                 reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&value),
                                                 24 };
        const std::vector<int>            preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
        const auto                        result = tetengo_lattice_node_toNode(
            &entry, 1, preceding_edge_costs.data(), preceding_edge_costs.size(), 5, 2424, nullptr);
        BOOST_TEST(!result);
    }
}

BOOST_AUTO_TEST_CASE(operator_equal)
{
    BOOST_TEST_PASSPOINT();

    {
        const std::vector<int> preceding_edge_costs_bos{};
        const auto             bos = tetengo::lattice::node::bos(&preceding_edge_costs_bos);

        const std::vector<int> preceding_edge_costs_eos{ 3, 1, 4, 1, 5, 9, 2, 6 };
        const auto             eos = tetengo::lattice::node::eos(1, &preceding_edge_costs_eos, 5, 42);

        const std::any               value1{ 42 };
        const std::vector<int>       preceding_edge_costs1{ 3, 1, 4, 1, 5, 9, 2, 6 };
        const tetengo::lattice::node node1{ "mizuho", &value1, 1, &preceding_edge_costs1, 5, 24, 2424 };

        const std::any               value2{ 42 };
        const std::vector<int>       preceding_edge_costs2{ 3, 1, 4, 1, 5, 9, 2, 6 };
        const tetengo::lattice::node node2{ "mizuho", &value2, 1, &preceding_edge_costs2, 5, 24, 2424 };

        BOOST_CHECK(bos == bos);
        BOOST_CHECK(bos != eos);
        BOOST_CHECK(bos != node1);
        BOOST_CHECK(node1 == node2);
    }

    {
        const std::vector<int> preceding_edge_costs_bos{};
        tetengo_lattice_node_t bos{};
        const auto             result_bos =
            tetengo_lattice_node_bos(preceding_edge_costs_bos.data(), preceding_edge_costs_bos.size(), &bos);
        BOOST_TEST_REQUIRE(result_bos);

        const std::vector<int> preceding_edge_costs_eos{ 3, 1, 4, 1, 5, 9, 2, 6 };
        tetengo_lattice_node_t eos{};
        const auto             result_eos =
            tetengo_lattice_node_eos(1, preceding_edge_costs_eos.data(), preceding_edge_costs_eos.size(), 5, 42, &eos);
        BOOST_TEST_REQUIRE(result_eos);

        const std::string_view       key1{ "mizuho" };
        const std::any               value1{ reinterpret_cast<const void*>("MIZUHO") };
        const std::vector<int>       preceding_edge_costs1{ 3, 1, 4, 1, 5, 9, 2, 6 };
        const tetengo_lattice_node_t node1{ { key1.data(), key1.length() },
                                            reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&value1),
                                            1,
                                            preceding_edge_costs1.data(),
                                            preceding_edge_costs1.size(),
                                            5,
                                            24,
                                            2424 };

        const std::string_view       key2{ "mizuho" };
        const std::any               value2{ reinterpret_cast<const void*>("MIZUHO") };
        const std::vector<int>       preceding_edge_costs2{ 3, 1, 4, 1, 5, 9, 2, 6 };
        const tetengo_lattice_node_t node2{ { key2.data(), key2.length() },
                                            reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&value2),
                                            1,
                                            preceding_edge_costs2.data(),
                                            preceding_edge_costs2.size(),
                                            5,
                                            24,
                                            2424 };

        BOOST_TEST(tetengo_lattice_node_equal(&bos, &bos));
        BOOST_TEST(!tetengo_lattice_node_equal(&bos, &eos));
        BOOST_TEST(!tetengo_lattice_node_equal(&bos, &node1));
        BOOST_TEST(tetengo_lattice_node_equal(&node1, &node2));
        BOOST_TEST(!tetengo_lattice_node_equal(nullptr, &node2));
        BOOST_TEST(!tetengo_lattice_node_equal(&node1, nullptr));
        BOOST_TEST(!tetengo_lattice_node_equal(nullptr, nullptr));
    }
}

BOOST_AUTO_TEST_CASE(key)
{
    BOOST_TEST_PASSPOINT();

    {
        const std::any               value{ 42 };
        const std::vector<int>       preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
        const tetengo::lattice::node node_{ "mizuho", &value, 1, &preceding_edge_costs, 5, 24, 2424 };

        BOOST_TEST(node_.key() == "mizuho");
    }

    {
        const std::string_view       key{ "mizuho" };
        const std::any               value{ reinterpret_cast<const void*>("MIZUHO") };
        const std::vector<int>       preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
        const tetengo_lattice_node_t node_{ { key.data(), key.length() },
                                            reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&value),
                                            1,
                                            preceding_edge_costs.data(),
                                            preceding_edge_costs.size(),
                                            5,
                                            24,
                                            2424 };

        BOOST_TEST(node_.key.p_head == key.data());
        BOOST_TEST(node_.key.length == key.length());
    }
}

BOOST_AUTO_TEST_CASE(value)
{
    BOOST_TEST_PASSPOINT();

    {
        const std::any               value{ 42 };
        const std::vector<int>       preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
        const tetengo::lattice::node node_{ "mizuho", &value, 1, &preceding_edge_costs, 5, 24, 2424 };

        BOOST_TEST(std::any_cast<int>(node_.value()) == 42);
    }

    {
        const std::string_view       key{ "mizuho" };
        const std::any               value{ reinterpret_cast<const void*>("MIZUHO") };
        const std::vector<int>       preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
        const tetengo_lattice_node_t node_{ { key.data(), key.length() },
                                            reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&value),
                                            1,
                                            preceding_edge_costs.data(),
                                            preceding_edge_costs.size(),
                                            5,
                                            24,
                                            2424 };

        BOOST_TEST(node_.value_handle == reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&value));
    }
}

BOOST_AUTO_TEST_CASE(preceding_step)
{
    BOOST_TEST_PASSPOINT();

    {
        const std::any               value{ 42 };
        const std::vector<int>       preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
        const tetengo::lattice::node node_{ "mizuho", &value, 1, &preceding_edge_costs, 5, 24, 2424 };

        BOOST_TEST(node_.preceding_step() == 1U);
    }

    {
        const std::string_view       key{ "mizuho" };
        const std::any               value{ reinterpret_cast<const void*>("MIZUHO") };
        const std::vector<int>       preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
        const tetengo_lattice_node_t node_{ { key.data(), key.length() },
                                            reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&value),
                                            1,
                                            preceding_edge_costs.data(),
                                            preceding_edge_costs.size(),
                                            5,
                                            24,
                                            2424 };

        BOOST_TEST(node_.preceding_step == 1U);
    }
}

BOOST_AUTO_TEST_CASE(preceding_edge_costs)
{
    BOOST_TEST_PASSPOINT();

    {
        const std::any               value{ 42 };
        const std::vector<int>       preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
        const tetengo::lattice::node node_{ "mizuho", &value, 1, &preceding_edge_costs, 5, 24, 2424 };

        BOOST_TEST(&node_.preceding_edge_costs() == &preceding_edge_costs);
    }

    {
        const std::string_view       key{ "mizuho" };
        const std::any               value{ reinterpret_cast<const void*>("MIZUHO") };
        const std::vector<int>       preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
        const tetengo_lattice_node_t node_{ { key.data(), key.length() },
                                            reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&value),
                                            1,
                                            preceding_edge_costs.data(),
                                            preceding_edge_costs.size(),
                                            5,
                                            24,
                                            2424 };

        BOOST_TEST(node_.p_preceding_edge_costs == preceding_edge_costs.data());
        BOOST_TEST(node_.preceding_edge_cost_count == preceding_edge_costs.size());
    }
}

BOOST_AUTO_TEST_CASE(best_preceding_node)
{
    BOOST_TEST_PASSPOINT();

    {
        const std::any               value{ 42 };
        const std::vector<int>       preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
        const tetengo::lattice::node node_{ "mizuho", &value, 1, &preceding_edge_costs, 5, 24, 2424 };

        BOOST_TEST(node_.best_preceding_node() == 5U);
    }

    {
        const std::string_view       key{ "mizuho" };
        const std::any               value{ reinterpret_cast<const void*>("MIZUHO") };
        const std::vector<int>       preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
        const tetengo_lattice_node_t node_{ { key.data(), key.length() },
                                            reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&value),
                                            1,
                                            preceding_edge_costs.data(),
                                            preceding_edge_costs.size(),
                                            5,
                                            24,
                                            2424 };

        BOOST_TEST(node_.best_preceding_node == 5U);
    }
}

BOOST_AUTO_TEST_CASE(node_cost)
{
    BOOST_TEST_PASSPOINT();

    {
        const std::any               value{ 42 };
        const std::vector<int>       preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
        const tetengo::lattice::node node_{ "mizuho", &value, 1, &preceding_edge_costs, 5, 24, 2424 };

        BOOST_TEST(node_.node_cost() == 24);
    }

    {
        const std::string_view       key{ "mizuho" };
        const std::any               value{ reinterpret_cast<const void*>("MIZUHO") };
        const std::vector<int>       preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
        const tetengo_lattice_node_t node_{ { key.data(), key.length() },
                                            reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&value),
                                            1,
                                            preceding_edge_costs.data(),
                                            preceding_edge_costs.size(),
                                            5,
                                            24,
                                            2424 };

        BOOST_TEST(node_.node_cost == 24);
    }
}

BOOST_AUTO_TEST_CASE(path_cost)
{
    BOOST_TEST_PASSPOINT();

    {
        const std::any               value{ 42 };
        const std::vector<int>       preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
        const tetengo::lattice::node node_{ "mizuho", &value, 1, &preceding_edge_costs, 5, 24, 2424 };

        BOOST_TEST(node_.path_cost() == 2424);
    }

    {
        const std::string_view       key{ "mizuho" };
        const std::any               value{ reinterpret_cast<const void*>("MIZUHO") };
        const std::vector<int>       preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
        const tetengo_lattice_node_t node_{ { key.data(), key.length() },
                                            reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&value),
                                            1,
                                            preceding_edge_costs.data(),
                                            preceding_edge_costs.size(),
                                            5,
                                            24,
                                            2424 };

        BOOST_TEST(node_.path_cost == 2424);
    }
}

BOOST_AUTO_TEST_CASE(is_bos)
{
    BOOST_TEST_PASSPOINT();

    {
        const std::vector<int> preceding_edge_costs{};
        BOOST_TEST(tetengo::lattice::node::bos(&preceding_edge_costs).is_bos());
    }
    {
        const std::vector<int> preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
        BOOST_TEST(!tetengo::lattice::node::eos(1, &preceding_edge_costs, 5, 42).is_bos());
    }
    {
        const std::any         value{ 42 };
        const std::vector<int> preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
        BOOST_TEST((!tetengo::lattice::node{ "mizuho", &value, 1, &preceding_edge_costs, 5, 24, 2424 }.is_bos()));
    }

    {
        tetengo_lattice_node_t bos{};
        const std::vector<int> preceding_edge_costs{};
        tetengo_lattice_node_bos(preceding_edge_costs.data(), preceding_edge_costs.size(), &bos);

        BOOST_TEST(tetengo_lattice_node_isBos(&bos));
    }
    {
        tetengo_lattice_node_t eos{};
        const std::vector<int> preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
        tetengo_lattice_node_eos(1, preceding_edge_costs.data(), preceding_edge_costs.size(), 5, 42, &eos);

        BOOST_TEST(!tetengo_lattice_node_isBos(&eos));
    }
    {
        const std::string_view       key{ "mizuho" };
        const std::any               value{ reinterpret_cast<const void*>("MIZUHO") };
        const std::vector<int>       preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
        const tetengo_lattice_node_t node_{ { key.data(), key.length() },
                                            reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&value),
                                            1,
                                            preceding_edge_costs.data(),
                                            preceding_edge_costs.size(),
                                            5,
                                            24,
                                            2424 };

        BOOST_TEST(!tetengo_lattice_node_isBos(&node_));
    }
    {
        BOOST_TEST(!tetengo_lattice_node_isBos(nullptr));
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
