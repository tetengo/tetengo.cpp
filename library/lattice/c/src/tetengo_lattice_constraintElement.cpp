/*! \file
    \brief A constraint element.

    $12026 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include <stddef.h>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>

#include <tetengo/lattice/constraintElement.h>
#include <tetengo/lattice/constraint_element.hpp>
#include <tetengo/lattice/entry.h>
#include <tetengo/lattice/input.h>
#include <tetengo/lattice/node.h>
#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/node_constraint_element.hpp>
#include <tetengo/lattice/wildcard_constraint_element.hpp>

#include "tetengo_lattice_constraintElement.hpp"
#include "tetengo_lattice_input.hpp"


tetengo_lattice_constraintElement_t*
tetengo_lattice_constraintElement_createNodeConstraintElement(const tetengo_lattice_node_t* const p_node)
{
    try
    {
        if (!p_node)
        {
            throw std::invalid_argument{ "p_node is NULL." };
        }

        const std::vector<int> cpp_preceding_edge_costs{
            p_node->p_preceding_edge_costs, p_node->p_preceding_edge_costs + p_node->preceding_edge_cost_count
        };
        const auto* const p_node_key = tetengo_lattice_entryView_createKeyOf(p_node->key_handle);
        BOOST_SCOPE_EXIT(p_node_key)
        {
            tetengo_lattice_input_destroy(p_node_key);
        }
        BOOST_SCOPE_EXIT_END;
        tetengo::lattice::node cpp_node{ p_node_key ? &p_node_key->cpp_input() : nullptr,
                                         reinterpret_cast<const std::any*>(p_node->value_handle),
                                         p_node->index_in_step,
                                         p_node->preceding_step,
                                         &cpp_preceding_edge_costs,
                                         p_node->best_preceding_node,
                                         p_node->node_cost,
                                         p_node->path_cost };

        auto p_cpp_constraint_element =
            std::make_unique<tetengo::lattice::node_constraint_element>(std::move(cpp_node));

        auto p_instance = std::make_unique<tetengo_lattice_constraintElement_t>(std::move(p_cpp_constraint_element));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

tetengo_lattice_constraintElement_t*
tetengo_lattice_constraintElement_createWildcardConstraintElement(const size_t preceding_step)
{
    try
    {
        auto p_cpp_constraint_element = std::make_unique<tetengo::lattice::wildcard_constraint_element>(preceding_step);

        auto p_instance = std::make_unique<tetengo_lattice_constraintElement_t>(std::move(p_cpp_constraint_element));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

void tetengo_lattice_constraintElement_destroy(const tetengo_lattice_constraintElement_t* const p_constraint_element)
{
    try
    {
        const std::unique_ptr<const tetengo_lattice_constraintElement_t> p_instance{ p_constraint_element };
    }
    catch (...)
    {}
}

int tetengo_lattice_constraintElement_matches(
    const tetengo_lattice_constraintElement_t* const p_constraint_element,
    const tetengo_lattice_node_t* const              p_node)
{
    try
    {
        if (!p_constraint_element)
        {
            throw std::invalid_argument{ "p_constraint_element is NULL." };
        }
        if (!p_node)
        {
            throw std::invalid_argument{ "p_node is NULL." };
        }

        const std::vector<int> cpp_preceding_edge_costs{
            p_node->p_preceding_edge_costs, p_node->p_preceding_edge_costs + p_node->preceding_edge_cost_count
        };
        const auto* const p_node_key = tetengo_lattice_entryView_createKeyOf(p_node->key_handle);
        BOOST_SCOPE_EXIT(p_node_key)
        {
            tetengo_lattice_input_destroy(p_node_key);
        }
        BOOST_SCOPE_EXIT_END;
        const tetengo::lattice::node cpp_node{ p_node_key ? &p_node_key->cpp_input() : nullptr,
                                               reinterpret_cast<const std::any*>(p_node->value_handle),
                                               p_node->index_in_step,
                                               p_node->preceding_step,
                                               &cpp_preceding_edge_costs,
                                               p_node->best_preceding_node,
                                               p_node->node_cost,
                                               p_node->path_cost };

        return p_constraint_element->p_cpp_constraint_element->matches(cpp_node);
    }
    catch (...)
    {
        return -1;
    }
}
