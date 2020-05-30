/*! \file
    \brief A node constraint element.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <any>
#include <string_view>
#include <utility>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/node_constraint_element.hpp>


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(lattice)
BOOST_AUTO_TEST_SUITE(node_constraint_element)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const std::any         element_node_value{ 42 };
        const std::vector<int> element_node_preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
        tetengo::lattice::node element_node{
            "mizuho", &element_node_value, 1, &element_node_preceding_edge_costs, 5, 24, 2424
        };
        const tetengo::lattice::node_constraint_element element{ std::move(element_node) };
    }
}

BOOST_AUTO_TEST_CASE(matches)
{
    BOOST_TEST_PASSPOINT();

    {
        const std::any         element_node_value{ 42 };
        const std::vector<int> element_node_preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
        tetengo::lattice::node element_node{
            "mizuho", &element_node_value, 1, &element_node_preceding_edge_costs, 5, 24, 2424
        };
        const tetengo::lattice::node_constraint_element element{ std::move(element_node) };

        {
            const std::any               value{ 42 };
            const std::vector<int>       preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
            const tetengo::lattice::node node_{ "mizuho", &value, 1, &preceding_edge_costs, 5, 24, 2424 };

            BOOST_TEST(element.matches(node_) == 0);
        }
        {
            const std::any               value{ 42 };
            const std::vector<int>       preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
            const tetengo::lattice::node node_{ "sakura", &value, 1, &preceding_edge_costs, 5, 24, 2424 };

            BOOST_TEST(element.matches(node_) < 0);
        }
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
