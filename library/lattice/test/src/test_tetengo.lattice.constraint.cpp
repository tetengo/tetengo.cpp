/*! \file
    \brief A constraint.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <memory>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/constraint.hpp>
#include <tetengo/lattice/constraint_element.hpp>
#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/node_constraint_element.hpp>


namespace
{
    const std::vector<int>& bos_preceding_edge_costs()
    {
        static const std::vector<int> singleton{};
        return singleton;
    }

    const std::vector<int>& preceding_edge_costs()
    {
        static const std::vector<int> singleton{ 1 };
        return singleton;
    }

    std::vector<std::unique_ptr<tetengo::lattice::constraint_element>> make_pattern_b_e()
    {
        std::vector<std::unique_ptr<tetengo::lattice::constraint_element>> pattern{};

        pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(
            tetengo::lattice::node::bos(&bos_preceding_edge_costs())));
        pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(
            tetengo::lattice::node::eos(0, &preceding_edge_costs(), 0, 0)));

        return pattern;
    }


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(lattice)
BOOST_AUTO_TEST_SUITE(constraint)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::constraint constraint_{ make_pattern_b_e() };
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
