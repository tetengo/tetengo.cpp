/*! \file
    \brief A node.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <any>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <string_view>

#include <boost/core/ignore_unused.hpp>
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
        const auto& bos = tetengo::lattice::node::bos();

        BOOST_TEST(bos.key() == tetengo::lattice::entry_view::bos_eos().key());
        BOOST_TEST(!bos.value().has_value());
        BOOST_TEST(bos.preceding_step() == std::numeric_limits<std::size_t>::max());
        BOOST_TEST(bos.best_preceding_node() == std::numeric_limits<std::size_t>::max());
        BOOST_TEST(bos.node_cost() == tetengo::lattice::entry_view::bos_eos().cost());
        BOOST_TEST(bos.path_cost() == 0);
    }

    {
        const auto* const p_bos = tetengo_lattice_node_bos();

        BOOST_TEST(p_bos->key.p_head == tetengo_lattice_entry_bosEos()->key.p_head);
        BOOST_TEST(p_bos->key.length == tetengo_lattice_entry_bosEos()->key.length);
        BOOST_TEST(!tetengo_lattice_entry_valueOf(p_bos->value_handle));
        BOOST_TEST(p_bos->preceding_step == std::numeric_limits<std::size_t>::max());
        BOOST_TEST(p_bos->best_preceding_node == std::numeric_limits<std::size_t>::max());
        BOOST_TEST(p_bos->node_cost == tetengo_lattice_entry_bosEos()->cost);
        BOOST_TEST(p_bos->path_cost == 0);
    }
}

BOOST_AUTO_TEST_CASE(eos)
{
    BOOST_TEST_PASSPOINT();

    {
        const auto eos = tetengo::lattice::node::eos(1, 5, 42);

        BOOST_TEST(eos.key() == tetengo::lattice::entry_view::bos_eos().key());
        BOOST_TEST(!eos.value().has_value());
        BOOST_TEST(eos.preceding_step() == 1U);
        BOOST_TEST(eos.best_preceding_node() == 5U);
        BOOST_TEST(eos.node_cost() == tetengo::lattice::entry_view::bos_eos().cost());
        BOOST_TEST(eos.path_cost() == 42);
    }

    {
        tetengo_lattice_node_t eos{};
        const auto             result = tetengo_lattice_node_eos(1, 5, 42, &eos);
        BOOST_TEST(result);

        BOOST_TEST(eos.key.p_head == tetengo_lattice_entry_bosEos()->key.p_head);
        BOOST_TEST(eos.key.length == tetengo_lattice_entry_bosEos()->key.length);
        BOOST_TEST(!tetengo_lattice_entry_valueOf(eos.value_handle));
        BOOST_TEST(eos.preceding_step == 1U);
        BOOST_TEST(eos.best_preceding_node == 5U);
        BOOST_TEST(eos.node_cost == tetengo_lattice_entry_bosEos()->cost);
        BOOST_TEST(eos.path_cost == 42);
    }
    {
        const auto result = tetengo_lattice_node_eos(1, 5, 42, nullptr);
        BOOST_TEST(!result);
    }
}

BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const std::any               value{ 42 };
        const tetengo::lattice::node node_{ "mizuho", &value, 1, 5, 24, 2424 };
    }
    {
        const std::any                     entry_value{ 42 };
        const tetengo::lattice::entry_view entry{ "mizuho", &entry_value, 24 };
        const tetengo::lattice::node       node_{ entry, 1, 5, 2424 };

        BOOST_TEST(node_.key() == "mizuho");
        BOOST_TEST(std::any_cast<int>(node_.value()) == 42);
        BOOST_TEST(node_.preceding_step() == 1U);
        BOOST_TEST(node_.best_preceding_node() == 5U);
        BOOST_TEST(node_.node_cost() == 24);
        BOOST_TEST(node_.path_cost() == 2424);
    }
    {
        BOOST_CHECK_THROW(const tetengo::lattice::node node_("mizuho", nullptr, 1, 5, 24, 2424), std::invalid_argument);
    }

    {
        const std::string_view       key{ "mizuho" };
        const std::any               value{ reinterpret_cast<const void*>("MIZUHO") };
        const tetengo_lattice_node_t node{
            { key.data(), key.length() }, reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&value), 1, 5, 24, 2424
        };
        boost::ignore_unused(node);
    }
    {
        const std::string_view            key{ "mizuho" };
        const std::any                    value{ reinterpret_cast<const void*>("MIZUHO") };
        const tetengo_lattice_entryView_t entry{ { key.data(), key.length() },
                                                 reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&value),
                                                 24 };
        tetengo_lattice_node_t            node{};
        const auto                        result = tetengo_lattice_node_toNode(&entry, 1, 5, 2424, &node);
        BOOST_TEST_REQUIRE(result);

        BOOST_TEST(node.key.p_head == entry.key.p_head);
        BOOST_TEST(node.key.length == entry.key.length);
        BOOST_TEST(node.value_handle == entry.value_handle);
        BOOST_TEST(node.preceding_step == 1U);
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
        tetengo_lattice_node_t            node{};
        const auto                        result = tetengo_lattice_node_toNode(&entry, 1, 5, 2424, &node);
        BOOST_TEST_REQUIRE(result);

        BOOST_TEST(node.key.p_head == entry.key.p_head);
        BOOST_TEST(node.key.length == entry.key.length);
        BOOST_TEST(!tetengo_lattice_entry_valueOf(node.value_handle));
        BOOST_TEST(node.preceding_step == 1U);
        BOOST_TEST(node.best_preceding_node == 5U);
        BOOST_TEST(node.node_cost == entry.cost);
        BOOST_TEST(node.path_cost == 2424);
    }
    {
        tetengo_lattice_node_t node{};
        const auto             result = tetengo_lattice_node_toNode(nullptr, 1, 5, 2424, &node);
        BOOST_TEST(!result);
    }
    {
        const std::string_view            key{ "mizuho" };
        const std::any                    value{ reinterpret_cast<const void*>("MIZUHO") };
        const tetengo_lattice_entryView_t entry{ { key.data(), key.length() },
                                                 reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&value),
                                                 24 };
        const auto                        result = tetengo_lattice_node_toNode(&entry, 1, 5, 2424, nullptr);
        BOOST_TEST(!result);
    }
}

BOOST_AUTO_TEST_CASE(key)
{
    BOOST_TEST_PASSPOINT();

    {
        const std::any               value{ 42 };
        const tetengo::lattice::node node_{ "mizuho", &value, 1, 5, 24, 2424 };

        BOOST_TEST(node_.key() == "mizuho");
    }

    {
        const std::string_view       key{ "mizuho" };
        const std::any               value{ reinterpret_cast<const void*>("MIZUHO") };
        const tetengo_lattice_node_t node_{
            { key.data(), key.length() }, reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&value), 1, 5, 24, 2424
        };

        BOOST_TEST(node_.key.p_head == key.data());
        BOOST_TEST(node_.key.length == key.length());
    }
}

BOOST_AUTO_TEST_CASE(value)
{
    BOOST_TEST_PASSPOINT();

    {
        const std::any               value{ 42 };
        const tetengo::lattice::node node_{ "mizuho", &value, 1, 5, 24, 2424 };

        BOOST_TEST(std::any_cast<int>(node_.value()) == 42);
    }

    {
        const std::string_view       key{ "mizuho" };
        const std::any               value{ reinterpret_cast<const void*>("MIZUHO") };
        const tetengo_lattice_node_t node_{
            { key.data(), key.length() }, reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&value), 1, 5, 24, 2424
        };

        BOOST_TEST(node_.value_handle == reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&value));
    }
}

BOOST_AUTO_TEST_CASE(preceding_step)
{
    BOOST_TEST_PASSPOINT();

    {
        const std::any               value{ 42 };
        const tetengo::lattice::node node_{ "mizuho", &value, 1, 5, 24, 2424 };

        BOOST_TEST(node_.preceding_step() == 1U);
    }

    {
        const std::string_view       key{ "mizuho" };
        const std::any               value{ reinterpret_cast<const void*>("MIZUHO") };
        const tetengo_lattice_node_t node_{
            { key.data(), key.length() }, reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&value), 1, 5, 24, 2424
        };

        BOOST_TEST(node_.preceding_step == 1U);
    }
}

BOOST_AUTO_TEST_CASE(best_preceding_node)
{
    BOOST_TEST_PASSPOINT();

    {
        const std::any               value{ 42 };
        const tetengo::lattice::node node_{ "mizuho", &value, 1, 5, 24, 2424 };

        BOOST_TEST(node_.best_preceding_node() == 5U);
    }

    {
        const std::string_view       key{ "mizuho" };
        const std::any               value{ reinterpret_cast<const void*>("MIZUHO") };
        const tetengo_lattice_node_t node_{
            { key.data(), key.length() }, reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&value), 1, 5, 24, 2424
        };

        BOOST_TEST(node_.best_preceding_node == 5U);
    }
}

BOOST_AUTO_TEST_CASE(node_cost)
{
    BOOST_TEST_PASSPOINT();

    {
        const std::any               value{ 42 };
        const tetengo::lattice::node node_{ "mizuho", &value, 1, 5, 24, 2424 };

        BOOST_TEST(node_.node_cost() == 24);
    }

    {
        const std::string_view       key{ "mizuho" };
        const std::any               value{ reinterpret_cast<const void*>("MIZUHO") };
        const tetengo_lattice_node_t node_{
            { key.data(), key.length() }, reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&value), 1, 5, 24, 2424
        };

        BOOST_TEST(node_.node_cost == 24);
    }
}

BOOST_AUTO_TEST_CASE(path_cost)
{
    BOOST_TEST_PASSPOINT();

    {
        const std::any               value{ 42 };
        const tetengo::lattice::node node_{ "mizuho", &value, 1, 5, 24, 2424 };

        BOOST_TEST(node_.path_cost() == 2424);
    }

    {
        const std::string_view       key{ "mizuho" };
        const std::any               value{ reinterpret_cast<const void*>("MIZUHO") };
        const tetengo_lattice_node_t node_{
            { key.data(), key.length() }, reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&value), 1, 5, 24, 2424
        };

        BOOST_TEST(node_.path_cost == 2424);
    }
}

BOOST_AUTO_TEST_CASE(is_bos)
{
    BOOST_TEST_PASSPOINT();

    {
        BOOST_TEST(tetengo::lattice::node::bos().is_bos());
    }
    {
        BOOST_TEST(!tetengo::lattice::node::eos(1, 5, 42).is_bos());
    }
    {
        const std::any value{ 42 };
        BOOST_TEST((!tetengo::lattice::node{ "mizuho", &value, 1, 5, 24, 2424 }.is_bos()));
    }

    {
        BOOST_TEST(tetengo_lattice_node_isBos(tetengo_lattice_node_bos()));
    }
    {
        tetengo_lattice_node_t eos{};
        tetengo_lattice_node_eos(1, 5, 42, &eos);

        BOOST_TEST(!tetengo_lattice_node_isBos(&eos));
    }
    {
        const std::string_view       key{ "mizuho" };
        const std::any               value{ reinterpret_cast<const void*>("MIZUHO") };
        const tetengo_lattice_node_t node_{
            { key.data(), key.length() }, reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&value), 1, 5, 24, 2424
        };

        BOOST_TEST(!tetengo_lattice_node_isBos(&node_));
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
