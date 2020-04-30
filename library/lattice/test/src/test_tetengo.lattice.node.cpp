/*! \file
    \brief A node.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <any>
#include <string_view>
#include <utility>

#include <boost/core/ignore_unused.hpp>
#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/node.h>
#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/string_view.h>


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(lattice)
BOOST_AUTO_TEST_SUITE(node)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::node node_{ "mizuho", 42, 1, 24, 2424 };
    }
    {
        const std::any               entry_value{ 42 };
        tetengo::lattice::entry_view entry{ "mizuho", &entry_value, 24 };
        const tetengo::lattice::node node_{ std::move(entry), 1, 2424 };

        BOOST_TEST(node_.key() == "mizuho");
        BOOST_TEST(std::any_cast<int>(node_.value()) == 42);
        BOOST_TEST(node_.preceding() == 1U);
        BOOST_TEST(node_.node_cost() == 24);
        BOOST_TEST(node_.path_cost() == 2424);
    }

    {
        const std::string_view     key{ "mizuho" };
        const auto                 value = 42;
        const tetengo_lattice_node node_{ { key.data(), key.length() }, &value, 1, 24, 2424 };
        boost::ignore_unused(node_);
    }
}

BOOST_AUTO_TEST_CASE(key)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::node node_{ "mizuho", 42, 1, 24, 2424 };

        BOOST_TEST(node_.key() == "mizuho");
    }

    {
        const std::string_view     key{ "mizuho" };
        const auto                 value = 42;
        const tetengo_lattice_node node_{ { key.data(), key.length() }, &value, 1, 24, 2424 };

        BOOST_TEST(node_.key.p_head == key.data());
        BOOST_TEST(node_.key.length == key.length());
    }
}

BOOST_AUTO_TEST_CASE(value)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::node node_{ "mizuho", 42, 1, 24, 2424 };

        BOOST_TEST(std::any_cast<int>(node_.value()) == 42);
    }

    {
        const std::string_view     key{ "mizuho" };
        const auto                 value = 42;
        const tetengo_lattice_node node_{ { key.data(), key.length() }, &value, 1, 24, 2424 };

        BOOST_TEST(node_.p_value == &value);
    }
}

BOOST_AUTO_TEST_CASE(preceding)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::node node_{ "mizuho", 42, 1, 24, 2424 };

        BOOST_TEST(node_.preceding() == 1U);
    }

    {
        const std::string_view     key{ "mizuho" };
        const auto                 value = 42;
        const tetengo_lattice_node node_{ { key.data(), key.length() }, &value, 1, 24, 2424 };

        BOOST_TEST(node_.preceding == 1U);
    }
}

BOOST_AUTO_TEST_CASE(node_cost)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::node node_{ "mizuho", 42, 1, 24, 2424 };

        BOOST_TEST(node_.node_cost() == 24);
    }

    {
        const std::string_view     key{ "mizuho" };
        const auto                 value = 42;
        const tetengo_lattice_node node_{ { key.data(), key.length() }, &value, 1, 24, 2424 };

        BOOST_TEST(node_.node_cost == 24);
    }
}

BOOST_AUTO_TEST_CASE(path_cost)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::node node_{ "mizuho", 42, 1, 24, 2424 };

        BOOST_TEST(node_.path_cost() == 2424);
    }

    {
        const std::string_view     key{ "mizuho" };
        const auto                 value = 42;
        const tetengo_lattice_node node_{ { key.data(), key.length() }, &value, 1, 24, 2424 };

        BOOST_TEST(node_.path_cost == 2424);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
