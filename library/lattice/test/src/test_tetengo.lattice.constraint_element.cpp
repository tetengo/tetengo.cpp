/*! \file
    \brief A constraint element.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <string_view>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/constraint_element.hpp>
#include <tetengo/lattice/node.hpp>


namespace
{
    class concrete_constraint_element : public tetengo::lattice::constraint_element
    {
    private:
        // virtual functions

        virtual int matches_impl(const tetengo::lattice::node& node_) const override
        {
            if (node_.preceding_step() < 3)
            {
                return -1;
            }
            else
            {
                return static_cast<int>(node_.preceding_step() - 3);
            }
        }
    };


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(lattice)
BOOST_AUTO_TEST_SUITE(constraint_element)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const concrete_constraint_element element{};
    }
}

BOOST_AUTO_TEST_CASE(matches)
{
    BOOST_TEST_PASSPOINT();

    {
        const concrete_constraint_element element{};

        {
            const std::any               value{ 42 };
            const std::vector<int>       preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
            const tetengo::lattice::node node_{ "mizuho", &value, 1, &preceding_edge_costs, 5, 24, 2424 };

            BOOST_TEST(element.matches(node_) < 0);
        }
        {
            const std::any               value{ 42 };
            const std::vector<int>       preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
            const tetengo::lattice::node node_{ "sakura", &value, 3, &preceding_edge_costs, 5, 24, 2424 };

            BOOST_TEST(element.matches(node_) == 0);
        }
        {
            const std::any               value{ 42 };
            const std::vector<int>       preceding_edge_costs{ 3, 1, 4, 1, 5, 9, 2, 6 };
            const tetengo::lattice::node node_{ "tsubame", &value, 5, &preceding_edge_costs, 5, 24, 2424 };

            BOOST_TEST(element.matches(node_) > 0);
        }
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
