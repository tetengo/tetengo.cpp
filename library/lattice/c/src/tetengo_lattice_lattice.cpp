/*! \file
    \brief A lattice.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <any>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <memory>
#include <string_view>
#include <utility>
#include <vector>

#include <stddef.h>

#include <tetengo/lattice/lattice.h>
#include <tetengo/lattice/lattice.hpp>
#include <tetengo/lattice/node.h> // IWYU pragma: keep
#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/stringView.h>
#include <tetengo/lattice/vocabulary.h>

#include "tetengo_lattice_lattice.hpp"
#include "tetengo_lattice_vocabulary.hpp" // IWYU pragma: keep


tetengo_lattice_lattice_t* tetengo_lattice_lattice_create(tetengo_lattice_vocabulary_t* const p_vocabulary)
{
    try
    {
        if (!p_vocabulary)
        {
            return nullptr;
        }

        auto p_cpp_lattice = std::make_unique<tetengo::lattice::lattice>(std::move(p_vocabulary->p_cpp_vocabulary));

        auto p_instance = std::make_unique<tetengo_lattice_lattice_t>(std::move(p_cpp_lattice), p_vocabulary);
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

void tetengo_lattice_lattice_destroy(const tetengo_lattice_lattice_t* const p_lattice)
{
    try
    {
        const std::unique_ptr<const tetengo_lattice_lattice_t> p_instance{ p_lattice };
    }
    catch (...)
    {}
}

size_t tetengo_lattice_lattice_stepCount(const tetengo_lattice_lattice_t* const p_lattice)
{
    try
    {
        if (!p_lattice)
        {
            return 0;
        }

        return p_lattice->p_cpp_lattice->step_count();
    }
    catch (...)
    {
        return 0;
    }
}

size_t tetengo_lattice_lattice_nodesAt(
    const tetengo_lattice_lattice_t* const p_lattice,
    const size_t                           step,
    tetengo_lattice_node_t* const          p_nodes)
{
    try
    {
        if (!p_lattice)
        {
            return 0;
        }
        if (step >= p_lattice->p_cpp_lattice->step_count())
        {
            return 0;
        }

        const auto& cpp_nodes = p_lattice->p_cpp_lattice->nodes_at(step);
        if (p_nodes)
        {
            for (auto i = static_cast<std::size_t>(0); i < cpp_nodes.size(); ++i)
            {
                p_nodes[i].key.p_head = cpp_nodes[i].key().data();
                p_nodes[i].key.length = cpp_nodes[i].key().length();
                p_nodes[i].value_handle = reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&cpp_nodes[i].value());
                p_nodes[i].preceding_step = cpp_nodes[i].preceding_step();
                p_nodes[i].p_preceding_edge_costs = cpp_nodes[i].preceding_edge_costs().data();
                p_nodes[i].preceding_edge_cost_count = cpp_nodes[i].preceding_edge_costs().size();
                p_nodes[i].best_preceding_node = cpp_nodes[i].best_preceding_node();
                p_nodes[i].node_cost = cpp_nodes[i].node_cost();
                p_nodes[i].path_cost = cpp_nodes[i].path_cost();
            }
        }
        return cpp_nodes.size();
    }
    catch (...)
    {
        return 0;
    }
}

int tetengo_lattice_lattice_pushBack(tetengo_lattice_lattice_t* const p_lattice, const char* const input)
{
    try
    {
        if (!p_lattice)
        {
            return 0;
        }
        if (!input)
        {
            return 0;
        }

        p_lattice->p_cpp_lattice->push_back(input);

        return 1;
    }
    catch (...)
    {
        return 0;
    }
}

size_t tetengo_lattice_lattice_settle(
    tetengo_lattice_lattice_t* const p_lattice,
    tetengo_lattice_node_t* const    p_eos_node,
    int* const                       p_preceding_edge_costs)
{
    try
    {
        if (!p_lattice)
        {
            return 0;
        }
        if (p_eos_node && !p_preceding_edge_costs)
        {
            return 0;
        }

        const auto cpp_eos_node_and_preceding_edge_costs = p_lattice->p_cpp_lattice->settle();

        if (p_preceding_edge_costs)
        {
            std::copy(
                std::begin(*cpp_eos_node_and_preceding_edge_costs.second),
                std::end(*cpp_eos_node_and_preceding_edge_costs.second),
                p_preceding_edge_costs);
        }

        if (p_eos_node)
        {
            assert(!cpp_eos_node_and_preceding_edge_costs.first.value().has_value());
            p_eos_node->key.p_head = cpp_eos_node_and_preceding_edge_costs.first.key().data();
            p_eos_node->key.length = cpp_eos_node_and_preceding_edge_costs.first.key().length();
            p_eos_node->value_handle = reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(
                &cpp_eos_node_and_preceding_edge_costs.first.value());
            p_eos_node->preceding_step = cpp_eos_node_and_preceding_edge_costs.first.preceding_step();
            p_eos_node->p_preceding_edge_costs = p_preceding_edge_costs;
            p_eos_node->preceding_edge_cost_count = cpp_eos_node_and_preceding_edge_costs.second->size();
            p_eos_node->best_preceding_node = cpp_eos_node_and_preceding_edge_costs.first.best_preceding_node();
            p_eos_node->node_cost = cpp_eos_node_and_preceding_edge_costs.first.node_cost();
            p_eos_node->path_cost = cpp_eos_node_and_preceding_edge_costs.first.path_cost();
        }

        return cpp_eos_node_and_preceding_edge_costs.second->size();
    }
    catch (...)
    {
        return 0;
    }
}
