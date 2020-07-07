/*! \file
    \brief A path.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
 */

#include <any>
#include <string_view>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/path.hpp>


namespace
{
    const std::any& node_value()
    {
        static const std::any singleton{ 42 };
        return singleton;
    }

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

    const std::vector<tetengo::lattice::node>& nodes_()
    {
        static const std::vector<tetengo::lattice::node> singleton{
            tetengo::lattice::node::bos(&bos_preceding_edge_costs()),
            tetengo::lattice::node{ "mizuho", &node_value(), 0, &preceding_edge_costs(), 0, 0, 0 },
            tetengo::lattice::node{ "sakura", &node_value(), 1, &preceding_edge_costs(), 0, 0, 0 },
            tetengo::lattice::node{ "tsubame", &node_value(), 2, &preceding_edge_costs(), 0, 0, 0 },
            tetengo::lattice::node::eos(3, &preceding_edge_costs(), 0, 0)
        };
        return singleton;
    }

}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(lattice)
BOOST_AUTO_TEST_SUITE(path)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::path path_{ nodes_(), 42 };
    }
}

BOOST_AUTO_TEST_CASE(nodes)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::path path_{ nodes_(), 42 };

        BOOST_CHECK(path_.nodes() == nodes_());
    }
}

BOOST_AUTO_TEST_CASE(cost)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::path path_{ nodes_(), 42 };

        BOOST_TEST(path_.cost() == 42U);
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
