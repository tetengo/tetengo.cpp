/*! \file
    \brief A node.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <any>
#include <string_view>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/node.hpp>


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(lattice)
BOOST_AUTO_TEST_SUITE(node)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::node node_{ "mizuho", 42, 24, 2424 };
    }
}

BOOST_AUTO_TEST_CASE(key)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::node node_{ "mizuho", 42, 24, 2424 };

        BOOST_TEST(node_.key() == "mizuho");
    }
}

BOOST_AUTO_TEST_CASE(value)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::node node_{ "mizuho", 42, 24, 2424 };

        BOOST_TEST(std::any_cast<int>(node_.value()) == 42);
    }
}

BOOST_AUTO_TEST_CASE(node_cost)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::node node_{ "mizuho", 42, 24, 2424 };

        BOOST_TEST(node_.node_cost() == 24);
    }
}

BOOST_AUTO_TEST_CASE(path_cost)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::node node_{ "mizuho", 42, 24, 2424 };

        BOOST_TEST(node_.path_cost() == 2424);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
