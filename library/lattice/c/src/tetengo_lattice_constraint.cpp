/*! \file
    \brief A constraint.

    Copyright (C) 2019-2023 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <any>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include <stddef.h>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>

#include <tetengo/lattice/constraint.h>
#include <tetengo/lattice/constraint.hpp>
#include <tetengo/lattice/constraintElement.h>
#include <tetengo/lattice/constraint_element.hpp>
#include <tetengo/lattice/entry.h>
#include <tetengo/lattice/input.h>
#include <tetengo/lattice/node.h>
#include <tetengo/lattice/node.hpp>

#include "tetengo_lattice_constraint.hpp"
#include "tetengo_lattice_constraintElement.hpp"
#include "tetengo_lattice_input.hpp"


namespace
{
    void to_cpp_path(
        const tetengo_lattice_node_t* const  p_path,
        const size_t                         path_length,
        std::vector<std::vector<int>>&       cpp_preceding_edge_cost_lists,
        std::vector<tetengo::lattice::node>& cpp_path)
    {
        cpp_preceding_edge_cost_lists.reserve(path_length);
        cpp_path.reserve(path_length);
        for (const auto* p_node = p_path; p_node != p_path + path_length; ++p_node)
        {
            const auto* const p_node_key = tetengo_lattice_entryView_createKeyOf(p_node->key_handle);
            BOOST_SCOPE_EXIT(p_node_key)
            {
                tetengo_lattice_input_destroy(p_node_key);
            }
            BOOST_SCOPE_EXIT_END;
            cpp_preceding_edge_cost_lists.emplace_back(
                p_node->p_preceding_edge_costs, p_node->p_preceding_edge_costs + p_node->preceding_edge_cost_count);
            cpp_path.emplace_back(
                p_node_key ? &p_node_key->cpp_input() : nullptr,
                reinterpret_cast<const std::any*>(p_node->value_handle),
                p_node->index_in_step,
                p_node->preceding_step,
                &cpp_preceding_edge_cost_lists.back(),
                p_node->best_preceding_node,
                p_node->node_cost,
                p_node->path_cost);
        };
    }


}


tetengo_lattice_constraint_t* tetengo_lattice_constraint_createEmpty()
{
    try
    {
        auto p_cpp_constraint = std::make_unique<tetengo::lattice::constraint>();

        auto p_instance = std::make_unique<tetengo_lattice_constraint_t>(std::move(p_cpp_constraint));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

tetengo_lattice_constraint_t* tetengo_lattice_constraint_create(
    tetengo_lattice_constraintElement_t* const* const pp_pattern,
    const size_t                                      pattern_length)
{
    try
    {
        if (!pp_pattern)
        {
            throw std::invalid_argument{ "pp_pattern is NULL." };
        }

        BOOST_SCOPE_EXIT(pp_pattern, pattern_length)
        {
            std::for_each(pp_pattern, pp_pattern + pattern_length, [](auto* const p_constraint_element) {
                tetengo_lattice_constraintElement_destroy(p_constraint_element);
            });
        }
        BOOST_SCOPE_EXIT_END;

        std::vector<std::unique_ptr<tetengo::lattice::constraint_element>> cpp_pattern;
        cpp_pattern.reserve(pattern_length);
        std::for_each(pp_pattern, pp_pattern + pattern_length, [&cpp_pattern](auto* const p_constraint_element) {
            if (!p_constraint_element)
            {
                throw std::invalid_argument{ "pp_pattern has NULL element." };
            }

            cpp_pattern.push_back(std::move(p_constraint_element->p_cpp_constraint_element));
        });

        auto p_cpp_constraint = std::make_unique<tetengo::lattice::constraint>(std::move(cpp_pattern));

        auto p_instance = std::make_unique<tetengo_lattice_constraint_t>(std::move(p_cpp_constraint));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

void tetengo_lattice_constraint_destroy(const tetengo_lattice_constraint_t* const p_constraint)
{
    try
    {
        const std::unique_ptr<const tetengo_lattice_constraint_t> p_instance{ p_constraint };
    }
    catch (...)
    {}
}

bool tetengo_lattice_constraint_matches(
    const tetengo_lattice_constraint_t* const p_constraint,
    const tetengo_lattice_node_t* const       p_reverse_path,
    const size_t                              path_length)
{
    try
    {
        if (!p_constraint)
        {
            throw std::invalid_argument{ "p_constraint is NULL." };
        }
        if (!p_reverse_path)
        {
            throw std::invalid_argument{ "p_reverse_path is NULL." };
        }

        std::vector<std::vector<int>>       cpp_preceding_edge_cost_lists{};
        std::vector<tetengo::lattice::node> cpp_reverse_path{};
        to_cpp_path(p_reverse_path, path_length, cpp_preceding_edge_cost_lists, cpp_reverse_path);

        return p_constraint->p_cpp_constraint->matches(cpp_reverse_path);
    }
    catch (...)
    {
        return false;
    }
}

bool tetengo_lattice_constraint_matchesTail(
    const tetengo_lattice_constraint_t* const p_constraint,
    const tetengo_lattice_node_t* const       p_reverse_tail_path,
    const size_t                              tail_path_length)
{
    try
    {
        if (!p_constraint)
        {
            throw std::invalid_argument{ "p_constraint is NULL." };
        }
        if (!p_reverse_tail_path)
        {
            throw std::invalid_argument{ "p_reverse_tail_path is NULL." };
        }

        std::vector<std::vector<int>>       cpp_preceding_edge_cost_lists{};
        std::vector<tetengo::lattice::node> cpp_reverse_tail_path{};
        to_cpp_path(p_reverse_tail_path, tail_path_length, cpp_preceding_edge_cost_lists, cpp_reverse_tail_path);

        return p_constraint->p_cpp_constraint->matches_tail(cpp_reverse_tail_path);
    }
    catch (...)
    {
        return false;
    }
}
