/*! \file
    \brief A constraint.

    Copyright (C) 2019-2023 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <any>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>
#include <boost/test/unit_test.hpp>

#include <tetengo/lattice/constraint.h>
#include <tetengo/lattice/constraint.hpp>
#include <tetengo/lattice/constraintElement.h>
#include <tetengo/lattice/constraint_element.hpp>
#include <tetengo/lattice/entry.h>
#include <tetengo/lattice/node.h>
#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/node_constraint_element.hpp>
#include <tetengo/lattice/string_input.hpp>
#include <tetengo/lattice/wildcard_constraint_element.hpp>


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

    const std::vector<tetengo::lattice::node>& path_b_e()
    {
        static const std::vector<tetengo::lattice::node> singleton{
            tetengo::lattice::node::bos(&bos_preceding_edge_costs()),
            tetengo::lattice::node::eos(0, &preceding_edge_costs(), 0, 0)
        };
        return singleton;
    }

    const std::vector<tetengo::lattice::node>& path_b_m_s_t_e()
    {
        static const tetengo::lattice::string_input      key_mizuho{ "mizuho" };
        static const tetengo::lattice::string_input      key_sakura{ "sakura" };
        static const tetengo::lattice::string_input      key_tsubame{ "tsubame" };
        static const std::vector<tetengo::lattice::node> singleton{
            tetengo::lattice::node::bos(&bos_preceding_edge_costs()),
            tetengo::lattice::node{ &key_mizuho, &node_value(), 0, 0, &preceding_edge_costs(), 0, 0, 0 },
            tetengo::lattice::node{ &key_sakura, &node_value(), 0, 1, &preceding_edge_costs(), 0, 0, 0 },
            tetengo::lattice::node{ &key_tsubame, &node_value(), 0, 2, &preceding_edge_costs(), 0, 0, 0 },
            tetengo::lattice::node::eos(3, &preceding_edge_costs(), 0, 0)
        };
        return singleton;
    }

    const std::vector<tetengo::lattice::node>& path_b_m_a_t_e()
    {
        static const tetengo::lattice::string_input      key_mizuho{ "mizuho" };
        static const tetengo::lattice::string_input      key_ariake{ "ariake" };
        static const tetengo::lattice::string_input      key_tsubame{ "tsubame" };
        static const std::vector<tetengo::lattice::node> singleton{
            tetengo::lattice::node::bos(&bos_preceding_edge_costs()),
            tetengo::lattice::node{ &key_mizuho, &node_value(), 0, 0, &preceding_edge_costs(), 0, 0, 0 },
            tetengo::lattice::node{ &key_ariake, &node_value(), 0, 1, &preceding_edge_costs(), 0, 0, 0 },
            tetengo::lattice::node{ &key_tsubame, &node_value(), 0, 2, &preceding_edge_costs(), 0, 0, 0 },
            tetengo::lattice::node::eos(3, &preceding_edge_costs(), 0, 0)
        };
        return singleton;
    }

    const std::vector<tetengo::lattice::node>& path_b_h_t_e()
    {
        static const tetengo::lattice::string_input      key_hinokuni{ "hinokuni" };
        static const tetengo::lattice::string_input      key_tsubame{ "tsubame" };
        static const std::vector<tetengo::lattice::node> singleton{
            tetengo::lattice::node::bos(&bos_preceding_edge_costs()),
            tetengo::lattice::node{ &key_hinokuni, &node_value(), 0, 0, &preceding_edge_costs(), 0, 0, 0 },
            tetengo::lattice::node{ &key_tsubame, &node_value(), 0, 2, &preceding_edge_costs(), 0, 0, 0 },
            tetengo::lattice::node::eos(3, &preceding_edge_costs(), 0, 0)
        };
        return singleton;
    }

    const std::vector<tetengo::lattice::node>& path_b_k_s_k_e()
    {
        static const tetengo::lattice::string_input      key_kamome{ "kamome" };
        static const tetengo::lattice::string_input      key_sakura{ "sakura" };
        static const tetengo::lattice::string_input      key_kumagawa{ "kumagawa" };
        static const std::vector<tetengo::lattice::node> singleton{
            tetengo::lattice::node::bos(&bos_preceding_edge_costs()),
            tetengo::lattice::node{ &key_kamome, &node_value(), 0, 0, &preceding_edge_costs(), 0, 0, 0 },
            tetengo::lattice::node{ &key_sakura, &node_value(), 0, 1, &preceding_edge_costs(), 0, 0, 0 },
            tetengo::lattice::node{ &key_kumagawa, &node_value(), 0, 2, &preceding_edge_costs(), 0, 0, 0 },
            tetengo::lattice::node::eos(3, &preceding_edge_costs(), 0, 0)
        };
        return singleton;
    }

    tetengo_lattice_node_t to_c_node(const tetengo::lattice::node& cpp_node)
    {
        tetengo_lattice_node_t c_node{};

        c_node.key_handle = reinterpret_cast<tetengo_lattice_entryView_keyHandle_t>(cpp_node.p_key());
        c_node.value_handle = reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(&cpp_node.value());
        c_node.index_in_step = cpp_node.index_in_step();
        c_node.preceding_step = cpp_node.preceding_step();
        c_node.p_preceding_edge_costs = std::data(cpp_node.preceding_edge_costs());
        c_node.preceding_edge_cost_count = std::size(cpp_node.preceding_edge_costs());
        c_node.best_preceding_node = cpp_node.best_preceding_node();
        c_node.node_cost = cpp_node.node_cost();
        c_node.path_cost = cpp_node.path_cost();

        return c_node;
    }

    std::vector<tetengo::lattice::node> reverse_path(const std::vector<tetengo::lattice::node>& path)
    {
        return std::vector<tetengo::lattice::node>{ std::rbegin(path), std::rend(path) };
    }

    std::vector<tetengo_lattice_node_t> to_c_reverse_path(const std::vector<tetengo::lattice::node>& cpp_path)
    {
        std::vector<tetengo_lattice_node_t> c_path{};
        c_path.reserve(std::size(cpp_path));
        std::transform(std::rbegin(cpp_path), std::rend(cpp_path), std::back_inserter(c_path), to_c_node);
        return c_path;
    }

    std::vector<std::unique_ptr<tetengo::lattice::constraint_element>> make_cpp_pattern_b_e()
    {
        std::vector<std::unique_ptr<tetengo::lattice::constraint_element>> pattern{};
        pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path_b_e()[0]));
        pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path_b_e()[1]));
        return pattern;
    }

    std::vector<tetengo_lattice_constraintElement_t*> make_c_pattern_b_e()
    {
        const tetengo_lattice_node_t node0 = to_c_node(path_b_e()[0]);
        const tetengo_lattice_node_t node1 = to_c_node(path_b_e()[1]);

        std::vector<tetengo_lattice_constraintElement_t*> pattern{};
        pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&node0));
        pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&node1));
        return pattern;
    }

    std::vector<tetengo_lattice_constraintElement_t*> make_c_pattern_b_n_e()
    {
        const tetengo_lattice_node_t node0 = to_c_node(path_b_e()[0]);
        const tetengo_lattice_node_t node2 = to_c_node(path_b_e()[1]);

        std::vector<tetengo_lattice_constraintElement_t*> pattern{};
        pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&node0));
        pattern.push_back(nullptr);
        pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&node2));
        return pattern;
    }

    std::vector<std::unique_ptr<tetengo::lattice::constraint_element>> make_cpp_pattern_b_m_s_t_e()
    {
        std::vector<std::unique_ptr<tetengo::lattice::constraint_element>> pattern{};
        pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path_b_m_s_t_e()[0]));
        pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path_b_m_s_t_e()[1]));
        pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path_b_m_s_t_e()[2]));
        pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path_b_m_s_t_e()[3]));
        pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path_b_m_s_t_e()[4]));
        return pattern;
    }

    std::vector<tetengo_lattice_constraintElement_t*> make_c_pattern_b_m_s_t_e()
    {
        const tetengo_lattice_node_t node0 = to_c_node(path_b_m_s_t_e()[0]);
        const tetengo_lattice_node_t node1 = to_c_node(path_b_m_s_t_e()[1]);
        const tetengo_lattice_node_t node2 = to_c_node(path_b_m_s_t_e()[2]);
        const tetengo_lattice_node_t node3 = to_c_node(path_b_m_s_t_e()[3]);
        const tetengo_lattice_node_t node4 = to_c_node(path_b_m_s_t_e()[4]);

        std::vector<tetengo_lattice_constraintElement_t*> pattern{};
        pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&node0));
        pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&node1));
        pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&node2));
        pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&node3));
        pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&node4));
        return pattern;
    }

    std::vector<std::unique_ptr<tetengo::lattice::constraint_element>> make_cpp_pattern_b_m_w_t_e()
    {
        std::vector<std::unique_ptr<tetengo::lattice::constraint_element>> pattern{};
        pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path_b_m_s_t_e()[0]));
        pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path_b_m_s_t_e()[1]));
        pattern.push_back(std::make_unique<tetengo::lattice::wildcard_constraint_element>(1));
        pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path_b_m_s_t_e()[3]));
        pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path_b_m_s_t_e()[4]));
        return pattern;
    }

    std::vector<tetengo_lattice_constraintElement_t*> make_c_pattern_b_m_w_t_e()
    {
        const tetengo_lattice_node_t node0 = to_c_node(path_b_m_s_t_e()[0]);
        const tetengo_lattice_node_t node1 = to_c_node(path_b_m_s_t_e()[1]);
        const tetengo_lattice_node_t node3 = to_c_node(path_b_m_s_t_e()[3]);
        const tetengo_lattice_node_t node4 = to_c_node(path_b_m_s_t_e()[4]);

        std::vector<tetengo_lattice_constraintElement_t*> pattern{};
        pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&node0));
        pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&node1));
        pattern.push_back(tetengo_lattice_constraintElement_createWildcardConstraintElement(1));
        pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&node3));
        pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&node4));
        return pattern;
    }

    std::vector<std::unique_ptr<tetengo::lattice::constraint_element>> make_cpp_pattern_b_w_t_e()
    {
        std::vector<std::unique_ptr<tetengo::lattice::constraint_element>> pattern{};
        pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path_b_m_s_t_e()[0]));
        pattern.push_back(std::make_unique<tetengo::lattice::wildcard_constraint_element>(0));
        pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path_b_m_s_t_e()[3]));
        pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path_b_m_s_t_e()[4]));
        return pattern;
    }

    std::vector<tetengo_lattice_constraintElement_t*> make_c_pattern_b_w_t_e()
    {
        const tetengo_lattice_node_t node0 = to_c_node(path_b_m_s_t_e()[0]);
        const tetengo_lattice_node_t node3 = to_c_node(path_b_m_s_t_e()[3]);
        const tetengo_lattice_node_t node4 = to_c_node(path_b_m_s_t_e()[4]);

        std::vector<tetengo_lattice_constraintElement_t*> pattern{};
        pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&node0));
        pattern.push_back(tetengo_lattice_constraintElement_createWildcardConstraintElement(0));
        pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&node3));
        pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&node4));
        return pattern;
    }

    std::vector<std::unique_ptr<tetengo::lattice::constraint_element>> make_cpp_pattern_b_w_s_w_e()
    {
        std::vector<std::unique_ptr<tetengo::lattice::constraint_element>> pattern{};
        pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path_b_m_s_t_e()[0]));
        pattern.push_back(std::make_unique<tetengo::lattice::wildcard_constraint_element>(0));
        pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path_b_m_s_t_e()[2]));
        pattern.push_back(std::make_unique<tetengo::lattice::wildcard_constraint_element>(2));
        pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path_b_m_s_t_e()[4]));
        return pattern;
    }

    std::vector<tetengo_lattice_constraintElement_t*> make_c_pattern_b_w_s_w_e()
    {
        const tetengo_lattice_node_t node0 = to_c_node(path_b_m_s_t_e()[0]);
        const tetengo_lattice_node_t node2 = to_c_node(path_b_m_s_t_e()[2]);
        const tetengo_lattice_node_t node4 = to_c_node(path_b_m_s_t_e()[4]);

        std::vector<tetengo_lattice_constraintElement_t*> pattern{};
        pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&node0));
        pattern.push_back(tetengo_lattice_constraintElement_createWildcardConstraintElement(0));
        pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&node2));
        pattern.push_back(tetengo_lattice_constraintElement_createWildcardConstraintElement(2));
        pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&node4));
        return pattern;
    }

    std::vector<std::unique_ptr<tetengo::lattice::constraint_element>> make_cpp_pattern_b_w_e()
    {
        std::vector<std::unique_ptr<tetengo::lattice::constraint_element>> pattern{};
        pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path_b_m_s_t_e()[0]));
        pattern.push_back(std::make_unique<tetengo::lattice::wildcard_constraint_element>(0));
        pattern.push_back(std::make_unique<tetengo::lattice::node_constraint_element>(path_b_m_s_t_e()[4]));
        return pattern;
    }

    std::vector<tetengo_lattice_constraintElement_t*> make_c_pattern_b_w_e()
    {
        const tetengo_lattice_node_t node0 = to_c_node(path_b_m_s_t_e()[0]);
        const tetengo_lattice_node_t node4 = to_c_node(path_b_m_s_t_e()[4]);

        std::vector<tetengo_lattice_constraintElement_t*> pattern{};
        pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&node0));
        pattern.push_back(tetengo_lattice_constraintElement_createWildcardConstraintElement(0));
        pattern.push_back(tetengo_lattice_constraintElement_createNodeConstraintElement(&node4));
        return pattern;
    }

    std::vector<std::unique_ptr<tetengo::lattice::constraint_element>> make_cpp_pattern_w()
    {
        std::vector<std::unique_ptr<tetengo::lattice::constraint_element>> pattern{};
        pattern.push_back(
            std::make_unique<tetengo::lattice::wildcard_constraint_element>(std::numeric_limits<std::size_t>::max()));
        return pattern;
    }

    std::vector<tetengo_lattice_constraintElement_t*> make_c_pattern_w()
    {
        std::vector<tetengo_lattice_constraintElement_t*> pattern{};
        pattern.push_back(
            tetengo_lattice_constraintElement_createWildcardConstraintElement(std::numeric_limits<size_t>::max()));
        return pattern;
    }

    std::vector<tetengo::lattice::node>
    make_tail(const std::vector<tetengo::lattice::node>& path, const std::size_t node_count)
    {
        assert(0 < node_count && node_count <= std::size(path));
        return std::vector<tetengo::lattice::node>{ std::next(std::begin(path), std::size(path) - node_count),
                                                    std::end(path) };
    }


}


BOOST_AUTO_TEST_SUITE(test_tetengo)
BOOST_AUTO_TEST_SUITE(lattice)
BOOST_AUTO_TEST_SUITE(constraint)


BOOST_AUTO_TEST_CASE(construction)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::constraint constraint_{};
    }
    {
        const tetengo::lattice::constraint constraint_{ make_cpp_pattern_b_e() };
    }

    {
        const auto* const p_constraint = tetengo_lattice_constraint_createEmpty();
        BOOST_SCOPE_EXIT(p_constraint)
        {
            tetengo_lattice_constraint_destroy(p_constraint);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_constraint);
    }
    {
        const auto        pattern = make_c_pattern_b_e();
        const auto* const p_constraint = tetengo_lattice_constraint_create(std::data(pattern), std::size(pattern));
        BOOST_SCOPE_EXIT(p_constraint)
        {
            tetengo_lattice_constraint_destroy(p_constraint);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST(p_constraint);
    }
    {
        const auto* const p_constraint = tetengo_lattice_constraint_create(nullptr, 0);
        BOOST_TEST(!p_constraint);
    }
    {
        const auto        pattern = make_c_pattern_b_n_e();
        const auto* const p_constraint = tetengo_lattice_constraint_create(std::data(pattern), std::size(pattern));
        BOOST_TEST(!p_constraint);
    }
}

BOOST_AUTO_TEST_CASE(matches)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::constraint constraint_{};

        BOOST_TEST(constraint_.matches(reverse_path(path_b_e())));
        BOOST_TEST(constraint_.matches(reverse_path(path_b_m_s_t_e())));
        BOOST_TEST(constraint_.matches(reverse_path(path_b_m_a_t_e())));
        BOOST_TEST(constraint_.matches(reverse_path(path_b_h_t_e())));
        BOOST_TEST(constraint_.matches(reverse_path(path_b_k_s_k_e())));
    }
    {
        const tetengo::lattice::constraint constraint_{ make_cpp_pattern_b_e() };

        BOOST_TEST(constraint_.matches(reverse_path(path_b_e())));
        BOOST_TEST(!constraint_.matches(reverse_path(path_b_m_s_t_e())));
        BOOST_TEST(!constraint_.matches(reverse_path(path_b_m_a_t_e())));
        BOOST_TEST(!constraint_.matches(reverse_path(path_b_h_t_e())));
        BOOST_TEST(!constraint_.matches(reverse_path(path_b_k_s_k_e())));
    }
    {
        const tetengo::lattice::constraint constraint_{ make_cpp_pattern_b_m_s_t_e() };

        BOOST_TEST(!constraint_.matches(reverse_path(path_b_e())));
        BOOST_TEST(constraint_.matches(reverse_path(path_b_m_s_t_e())));
        BOOST_TEST(!constraint_.matches(reverse_path(path_b_m_a_t_e())));
        BOOST_TEST(!constraint_.matches(reverse_path(path_b_h_t_e())));
        BOOST_TEST(!constraint_.matches(reverse_path(path_b_k_s_k_e())));
    }
    {
        const tetengo::lattice::constraint constraint_{ make_cpp_pattern_b_m_w_t_e() };

        BOOST_TEST(!constraint_.matches(reverse_path(path_b_e())));
        BOOST_TEST(constraint_.matches(reverse_path(path_b_m_s_t_e())));
        BOOST_TEST(constraint_.matches(reverse_path(path_b_m_a_t_e())));
        BOOST_TEST(!constraint_.matches(reverse_path(path_b_h_t_e())));
        BOOST_TEST(!constraint_.matches(reverse_path(path_b_k_s_k_e())));
    }
    {
        const tetengo::lattice::constraint constraint_{ make_cpp_pattern_b_w_t_e() };

        BOOST_TEST(!constraint_.matches(reverse_path(path_b_e())));
        BOOST_TEST(constraint_.matches(reverse_path(path_b_m_s_t_e())));
        BOOST_TEST(constraint_.matches(reverse_path(path_b_m_a_t_e())));
        BOOST_TEST(constraint_.matches(reverse_path(path_b_h_t_e())));
        BOOST_TEST(!constraint_.matches(reverse_path(path_b_k_s_k_e())));
    }
    {
        const tetengo::lattice::constraint constraint_{ make_cpp_pattern_b_w_s_w_e() };

        BOOST_TEST(!constraint_.matches(reverse_path(path_b_e())));
        BOOST_TEST(constraint_.matches(reverse_path(path_b_m_s_t_e())));
        BOOST_TEST(!constraint_.matches(reverse_path(path_b_m_a_t_e())));
        BOOST_TEST(!constraint_.matches(reverse_path(path_b_h_t_e())));
        BOOST_TEST(constraint_.matches(reverse_path(path_b_k_s_k_e())));
    }
    {
        const tetengo::lattice::constraint constraint_{ make_cpp_pattern_b_w_e() };

        BOOST_TEST(!constraint_.matches(reverse_path(path_b_e())));
        BOOST_TEST(constraint_.matches(reverse_path(path_b_m_s_t_e())));
        BOOST_TEST(constraint_.matches(reverse_path(path_b_m_a_t_e())));
        BOOST_TEST(constraint_.matches(reverse_path(path_b_h_t_e())));
        BOOST_TEST(constraint_.matches(reverse_path(path_b_k_s_k_e())));
    }
    {
        const tetengo::lattice::constraint constraint_{ make_cpp_pattern_w() };

        BOOST_TEST(constraint_.matches(reverse_path(path_b_e())));
        BOOST_TEST(constraint_.matches(reverse_path(path_b_m_s_t_e())));
        BOOST_TEST(constraint_.matches(reverse_path(path_b_m_a_t_e())));
        BOOST_TEST(constraint_.matches(reverse_path(path_b_h_t_e())));
        BOOST_TEST(constraint_.matches(reverse_path(path_b_k_s_k_e())));
    }

    {
        const auto* const p_constraint = tetengo_lattice_constraint_createEmpty();
        BOOST_SCOPE_EXIT(p_constraint)
        {
            tetengo_lattice_constraint_destroy(p_constraint);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_constraint);

        {
            const auto path = to_c_reverse_path(path_b_e());
            BOOST_TEST(tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_m_s_t_e());
            BOOST_TEST(tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_m_a_t_e());
            BOOST_TEST(tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_h_t_e());
            BOOST_TEST(tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_k_s_k_e());
            BOOST_TEST(tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
    }
    {
        const auto        pattern = make_c_pattern_b_e();
        const auto* const p_constraint = tetengo_lattice_constraint_create(std::data(pattern), std::size(pattern));
        BOOST_SCOPE_EXIT(p_constraint)
        {
            tetengo_lattice_constraint_destroy(p_constraint);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_constraint);

        {
            const auto path = to_c_reverse_path(path_b_e());
            BOOST_TEST(tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_m_s_t_e());
            BOOST_TEST(!tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_m_a_t_e());
            BOOST_TEST(!tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_h_t_e());
            BOOST_TEST(!tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_k_s_k_e());
            BOOST_TEST(!tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
    }
    {
        const auto        pattern = make_c_pattern_b_m_s_t_e();
        const auto* const p_constraint = tetengo_lattice_constraint_create(std::data(pattern), std::size(pattern));
        BOOST_SCOPE_EXIT(p_constraint)
        {
            tetengo_lattice_constraint_destroy(p_constraint);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_constraint);

        {
            const auto path = to_c_reverse_path(path_b_e());
            BOOST_TEST(!tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_m_s_t_e());
            BOOST_TEST(tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_m_a_t_e());
            BOOST_TEST(!tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_h_t_e());
            BOOST_TEST(!tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_k_s_k_e());
            BOOST_TEST(!tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
    }
    {
        const auto        pattern = make_c_pattern_b_m_w_t_e();
        const auto* const p_constraint = tetengo_lattice_constraint_create(std::data(pattern), std::size(pattern));
        BOOST_SCOPE_EXIT(p_constraint)
        {
            tetengo_lattice_constraint_destroy(p_constraint);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_constraint);

        {
            const auto path = to_c_reverse_path(path_b_e());
            BOOST_TEST(!tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_m_s_t_e());
            BOOST_TEST(tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_m_a_t_e());
            BOOST_TEST(tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_h_t_e());
            BOOST_TEST(!tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_k_s_k_e());
            BOOST_TEST(!tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
    }
    {
        const auto        pattern = make_c_pattern_b_w_t_e();
        const auto* const p_constraint = tetengo_lattice_constraint_create(std::data(pattern), std::size(pattern));
        BOOST_SCOPE_EXIT(p_constraint)
        {
            tetengo_lattice_constraint_destroy(p_constraint);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_constraint);

        {
            const auto path = to_c_reverse_path(path_b_e());
            BOOST_TEST(!tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_m_s_t_e());
            BOOST_TEST(tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_m_a_t_e());
            BOOST_TEST(tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_h_t_e());
            BOOST_TEST(tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_k_s_k_e());
            BOOST_TEST(!tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
    }
    {
        const auto        pattern = make_c_pattern_b_w_s_w_e();
        const auto* const p_constraint = tetengo_lattice_constraint_create(std::data(pattern), std::size(pattern));
        BOOST_SCOPE_EXIT(p_constraint)
        {
            tetengo_lattice_constraint_destroy(p_constraint);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_constraint);

        {
            const auto path = to_c_reverse_path(path_b_e());
            BOOST_TEST(!tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_m_s_t_e());
            BOOST_TEST(tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_m_a_t_e());
            BOOST_TEST(!tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_h_t_e());
            BOOST_TEST(!tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_k_s_k_e());
            BOOST_TEST(tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
    }
    {
        const auto        pattern = make_c_pattern_b_w_e();
        const auto* const p_constraint = tetengo_lattice_constraint_create(std::data(pattern), std::size(pattern));
        BOOST_SCOPE_EXIT(p_constraint)
        {
            tetengo_lattice_constraint_destroy(p_constraint);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_constraint);

        {
            const auto path = to_c_reverse_path(path_b_e());
            BOOST_TEST(!tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_m_s_t_e());
            BOOST_TEST(tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_m_a_t_e());
            BOOST_TEST(tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_h_t_e());
            BOOST_TEST(tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_k_s_k_e());
            BOOST_TEST(tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
    }
    {
        const auto        pattern = make_c_pattern_w();
        const auto* const p_constraint = tetengo_lattice_constraint_create(std::data(pattern), std::size(pattern));
        BOOST_SCOPE_EXIT(p_constraint)
        {
            tetengo_lattice_constraint_destroy(p_constraint);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_constraint);

        {
            const auto path = to_c_reverse_path(path_b_e());
            BOOST_TEST(tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_m_s_t_e());
            BOOST_TEST(tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_m_a_t_e());
            BOOST_TEST(tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_h_t_e());
            BOOST_TEST(tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
        {
            const auto path = to_c_reverse_path(path_b_k_s_k_e());
            BOOST_TEST(tetengo_lattice_constraint_matches(p_constraint, std::data(path), std::size(path)));
        }
    }
    {
        const auto path = to_c_reverse_path(path_b_e());
        BOOST_TEST(!tetengo_lattice_constraint_matches(nullptr, std::data(path), std::size(path)));
    }
    {
        const auto* const p_constraint = tetengo_lattice_constraint_createEmpty();
        BOOST_SCOPE_EXIT(p_constraint)
        {
            tetengo_lattice_constraint_destroy(p_constraint);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_constraint);

        BOOST_TEST(!tetengo_lattice_constraint_matches(p_constraint, nullptr, 3));
    }
}

BOOST_AUTO_TEST_CASE(matches_tail_cpp)
{
    BOOST_TEST_PASSPOINT();

    {
        const tetengo::lattice::constraint constraint_{};

        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_e(), 2))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 2))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 3))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 4))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 5))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 2))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 3))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 4))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 5))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_h_t_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_h_t_e(), 2))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_h_t_e(), 3))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_h_t_e(), 4))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_k_s_k_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_k_s_k_e(), 2))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_k_s_k_e(), 3))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_k_s_k_e(), 4))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_k_s_k_e(), 5))));
    }
    {
        const tetengo::lattice::constraint constraint_{ make_cpp_pattern_b_e() };

        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_e(), 2))));
        BOOST_TEST(!constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 1))));
        BOOST_TEST(!constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 1))));
        BOOST_TEST(!constraint_.matches_tail(reverse_path(make_tail(path_b_h_t_e(), 1))));
        BOOST_TEST(!constraint_.matches_tail(reverse_path(make_tail(path_b_k_s_k_e(), 1))));
    }
    {
        const tetengo::lattice::constraint constraint_{ make_cpp_pattern_b_m_s_t_e() };

        BOOST_TEST(!constraint_.matches_tail(reverse_path(make_tail(path_b_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 2))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 3))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 4))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 5))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 2))));
        BOOST_TEST(!constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 3))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_h_t_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_h_t_e(), 2))));
        BOOST_TEST(!constraint_.matches_tail(reverse_path(make_tail(path_b_h_t_e(), 3))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_k_s_k_e(), 1))));
        BOOST_TEST(!constraint_.matches_tail(reverse_path(make_tail(path_b_k_s_k_e(), 2))));
    }
    {
        const tetengo::lattice::constraint constraint_{ make_cpp_pattern_b_m_w_t_e() };

        BOOST_TEST(!constraint_.matches_tail(reverse_path(make_tail(path_b_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 2))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 3))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 4))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 5))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 2))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 3))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 4))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 5))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_h_t_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_h_t_e(), 2))));
        BOOST_TEST(!constraint_.matches_tail(reverse_path(make_tail(path_b_h_t_e(), 3))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_k_s_k_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_k_s_k_e(), 1))));
        BOOST_TEST(!constraint_.matches_tail(reverse_path(make_tail(path_b_k_s_k_e(), 2))));
    }
    {
        const tetengo::lattice::constraint constraint_{ make_cpp_pattern_b_w_t_e() };

        BOOST_TEST(!constraint_.matches_tail(reverse_path(make_tail(path_b_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 2))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 3))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 4))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 5))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 2))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 3))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 4))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 5))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_h_t_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_h_t_e(), 2))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_h_t_e(), 3))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_h_t_e(), 4))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_k_s_k_e(), 1))));
        BOOST_TEST(!constraint_.matches_tail(reverse_path(make_tail(path_b_k_s_k_e(), 2))));
    }
    {
        const tetengo::lattice::constraint constraint_{ make_cpp_pattern_b_w_s_w_e() };

        BOOST_TEST(!constraint_.matches_tail(reverse_path(make_tail(path_b_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 2))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 3))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 4))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 5))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 2))));
        BOOST_TEST(!constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 3))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_h_t_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_h_t_e(), 2))));
        BOOST_TEST(!constraint_.matches_tail(reverse_path(make_tail(path_b_h_t_e(), 3))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_k_s_k_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_k_s_k_e(), 2))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_k_s_k_e(), 3))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_k_s_k_e(), 4))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_k_s_k_e(), 5))));
    }
    {
        const tetengo::lattice::constraint constraint_{ make_cpp_pattern_b_w_e() };

        BOOST_TEST(!constraint_.matches_tail(reverse_path(make_tail(path_b_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 2))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 3))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 4))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 5))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 2))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 3))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 4))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 5))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_h_t_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_h_t_e(), 2))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_h_t_e(), 3))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_h_t_e(), 4))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_k_s_k_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_k_s_k_e(), 2))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_k_s_k_e(), 3))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_k_s_k_e(), 4))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_k_s_k_e(), 5))));
    }
    {
        const tetengo::lattice::constraint constraint_{ make_cpp_pattern_w() };

        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_e(), 2))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 2))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 3))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 4))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_s_t_e(), 5))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 2))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 3))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 4))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_m_a_t_e(), 5))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_h_t_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_h_t_e(), 2))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_h_t_e(), 3))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_h_t_e(), 4))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_k_s_k_e(), 1))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_k_s_k_e(), 2))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_k_s_k_e(), 3))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_k_s_k_e(), 4))));
        BOOST_TEST(constraint_.matches_tail(reverse_path(make_tail(path_b_k_s_k_e(), 5))));
    }
}

BOOST_AUTO_TEST_CASE(matches_tail_c)
{
    BOOST_TEST_PASSPOINT();

    {
        const auto* const p_constraint = tetengo_lattice_constraint_createEmpty();
        BOOST_SCOPE_EXIT(p_constraint)
        {
            tetengo_lattice_constraint_destroy(p_constraint);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_constraint);

        {
            const auto cpp_tail_path = make_tail(path_b_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_e(), 2);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 2);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 3);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 4);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 5);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 2);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 3);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 4);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 5);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_h_t_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_h_t_e(), 2);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_h_t_e(), 3);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_h_t_e(), 4);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_k_s_k_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_k_s_k_e(), 2);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_k_s_k_e(), 3);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_k_s_k_e(), 4);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_k_s_k_e(), 5);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
    }
    {
        const auto        pattern = make_c_pattern_b_e();
        const auto* const p_constraint = tetengo_lattice_constraint_create(std::data(pattern), std::size(pattern));
        BOOST_SCOPE_EXIT(p_constraint)
        {
            tetengo_lattice_constraint_destroy(p_constraint);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_constraint);

        {
            const auto cpp_tail_path = make_tail(path_b_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_e(), 2);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(!tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(!tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_h_t_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(!tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_k_s_k_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(!tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
    }
    {
        const auto        pattern = make_c_pattern_b_m_s_t_e();
        const auto* const p_constraint = tetengo_lattice_constraint_create(std::data(pattern), std::size(pattern));
        BOOST_SCOPE_EXIT(p_constraint)
        {
            tetengo_lattice_constraint_destroy(p_constraint);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_constraint);

        {
            const auto cpp_tail_path = make_tail(path_b_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(!tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 2);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 3);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 4);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 5);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 2);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 3);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(!tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_h_t_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_h_t_e(), 2);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_h_t_e(), 3);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(!tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_k_s_k_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_k_s_k_e(), 2);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(!tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
    }
    {
        const auto        pattern = make_c_pattern_b_m_w_t_e();
        const auto* const p_constraint = tetengo_lattice_constraint_create(std::data(pattern), std::size(pattern));
        BOOST_SCOPE_EXIT(p_constraint)
        {
            tetengo_lattice_constraint_destroy(p_constraint);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_constraint);

        {
            const auto cpp_tail_path = make_tail(path_b_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(!tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 2);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 3);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 4);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 5);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 2);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 3);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 4);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 5);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_h_t_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_h_t_e(), 2);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_h_t_e(), 3);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(!tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_k_s_k_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_k_s_k_e(), 2);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(!tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
    }
    {
        const auto        pattern = make_c_pattern_b_w_t_e();
        const auto* const p_constraint = tetengo_lattice_constraint_create(std::data(pattern), std::size(pattern));
        BOOST_SCOPE_EXIT(p_constraint)
        {
            tetengo_lattice_constraint_destroy(p_constraint);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_constraint);

        {
            const auto cpp_tail_path = make_tail(path_b_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(!tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 2);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 3);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 4);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 5);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 2);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 3);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 4);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 5);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_h_t_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_h_t_e(), 2);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_h_t_e(), 3);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_h_t_e(), 4);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_k_s_k_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_k_s_k_e(), 2);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(!tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
    }
    {
        const auto        pattern = make_c_pattern_b_w_s_w_e();
        const auto* const p_constraint = tetengo_lattice_constraint_create(std::data(pattern), std::size(pattern));
        BOOST_SCOPE_EXIT(p_constraint)
        {
            tetengo_lattice_constraint_destroy(p_constraint);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_constraint);

        {
            const auto cpp_tail_path = make_tail(path_b_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(!tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 2);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 3);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 4);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 5);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 2);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 3);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(!tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_h_t_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_h_t_e(), 2);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_h_t_e(), 3);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(!tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_k_s_k_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_k_s_k_e(), 2);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_k_s_k_e(), 3);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_k_s_k_e(), 4);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_k_s_k_e(), 5);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
    }
    {
        const auto        pattern = make_c_pattern_b_w_e();
        const auto* const p_constraint = tetengo_lattice_constraint_create(std::data(pattern), std::size(pattern));
        BOOST_SCOPE_EXIT(p_constraint)
        {
            tetengo_lattice_constraint_destroy(p_constraint);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_constraint);

        {
            const auto cpp_tail_path = make_tail(path_b_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(!tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 2);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 3);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 4);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 5);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 2);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 3);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 4);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 5);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_h_t_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_h_t_e(), 2);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_h_t_e(), 3);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_h_t_e(), 4);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_k_s_k_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_k_s_k_e(), 2);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_k_s_k_e(), 3);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_k_s_k_e(), 4);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_k_s_k_e(), 5);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
    }
    {
        const auto        pattern = make_c_pattern_w();
        const auto* const p_constraint = tetengo_lattice_constraint_create(std::data(pattern), std::size(pattern));
        BOOST_SCOPE_EXIT(p_constraint)
        {
            tetengo_lattice_constraint_destroy(p_constraint);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_constraint);

        {
            const auto cpp_tail_path = make_tail(path_b_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_e(), 2);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 2);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 3);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 4);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_s_t_e(), 5);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 2);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 3);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 4);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_m_a_t_e(), 5);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_h_t_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_h_t_e(), 2);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_h_t_e(), 3);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_h_t_e(), 4);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_k_s_k_e(), 1);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_k_s_k_e(), 2);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_k_s_k_e(), 3);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_k_s_k_e(), 4);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
        {
            const auto cpp_tail_path = make_tail(path_b_k_s_k_e(), 5);
            const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
            BOOST_TEST(tetengo_lattice_constraint_matchesTail(
                p_constraint, std::data(reverse_tail_path), std::size(reverse_tail_path)));
        }
    }
    {
        const auto cpp_tail_path = make_tail(path_b_e(), 1);
        const auto reverse_tail_path = to_c_reverse_path(cpp_tail_path);
        BOOST_TEST(!tetengo_lattice_constraint_matchesTail(
            nullptr, std::data(reverse_tail_path), std::size(reverse_tail_path)));
    }
    {
        const auto* const p_constraint = tetengo_lattice_constraint_createEmpty();
        BOOST_SCOPE_EXIT(p_constraint)
        {
            tetengo_lattice_constraint_destroy(p_constraint);
        }
        BOOST_SCOPE_EXIT_END;
        BOOST_TEST_REQUIRE(p_constraint);

        BOOST_TEST(!tetengo_lattice_constraint_matchesTail(p_constraint, nullptr, 3));
    }
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
