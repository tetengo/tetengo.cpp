/*! \file
    \brief A node.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <stdexcept>
#include <string_view>
#include <vector>

#include <stddef.h>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>

#include <tetengo/lattice/entry.h>
#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/node.h>
#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/stringView.h>


int tetengo_lattice_node_bos(
    const int* const              p_preceding_edge_costs,
    const size_t                  preceding_edge_count,
    tetengo_lattice_node_t* const p_bos)
{
    try
    {
        if (preceding_edge_count > 0 && !p_preceding_edge_costs)
        {
            throw std::invalid_argument{
                "p_preceding_edge_costs is NULL though preceding_edge_count is greater than 0."
            };
        }
        if (!p_bos)
        {
            throw std::invalid_argument{ "p_bos is NULL." };
        }

        const std::vector<int> cpp_preceding_edge_costs{};
        const auto             cpp_bos = tetengo::lattice::node::bos(&cpp_preceding_edge_costs);

        p_bos->key_handle = tetengo_lattice_entryView_bosEos()->key_handle;
        p_bos->value_handle = reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(&cpp_bos.value());
        p_bos->preceding_step = cpp_bos.preceding_step();
        p_bos->p_preceding_edge_costs = p_preceding_edge_costs;
        p_bos->preceding_edge_cost_count = preceding_edge_count;
        p_bos->best_preceding_node = cpp_bos.best_preceding_node();
        p_bos->node_cost = cpp_bos.node_cost();
        p_bos->path_cost = cpp_bos.path_cost();

        return 1;
    }
    catch (...)
    {
        return 0;
    }
}

int tetengo_lattice_node_eos(
    const size_t                  preceding_step,
    const int* const              p_preceding_edge_costs,
    const size_t                  preceding_edge_count,
    const size_t                  best_preceding_node,
    const int                     path_cost,
    tetengo_lattice_node_t* const p_eos)
{
    try
    {
        if (preceding_edge_count > 0 && !p_preceding_edge_costs)
        {
            throw std::invalid_argument{
                "p_preceding_edge_costs is NULL though preceding_edge_count is greater than 0."
            };
        }
        if (!p_eos)
        {
            throw std::invalid_argument{ "p_eos is NULL." };
        }

        const std::vector<int> cpp_preceding_edge_costs{};
        const auto             cpp_eos =
            tetengo::lattice::node::eos(preceding_step, &cpp_preceding_edge_costs, best_preceding_node, path_cost);

        p_eos->key_handle = tetengo_lattice_entryView_bosEos()->key_handle;
        p_eos->value_handle = reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(&cpp_eos.value());
        p_eos->preceding_step = cpp_eos.preceding_step();
        p_eos->p_preceding_edge_costs = p_preceding_edge_costs;
        p_eos->preceding_edge_cost_count = preceding_edge_count;
        p_eos->best_preceding_node = cpp_eos.best_preceding_node();
        p_eos->node_cost = cpp_eos.node_cost();
        p_eos->path_cost = cpp_eos.path_cost();

        return 1;
    }
    catch (...)
    {
        return 0;
    }
}

int tetengo_lattice_node_toNode(
    const tetengo_lattice_entryView_t* const p_entry,
    const size_t                             preceding_step,
    const int* const                         p_preceding_edge_costs,
    const size_t                             preceding_edge_count,
    const size_t                             best_preceding_node,
    const int                                path_cost,
    tetengo_lattice_node_t* const            p_node)
{
    try
    {
        if (!p_entry)
        {
            throw std::invalid_argument{ "p_entry is NULL." };
        }
        if (preceding_edge_count > 0 && !p_preceding_edge_costs)
        {
            throw std::invalid_argument{
                "p_preceding_edge_costs is NULL though preceding_edge_count is greater than 0."
            };
        }
        if (!p_node)
        {
            throw std::invalid_argument{ "p_node is NULL." };
        }

        const auto* const p_entry_key = tetengo_lattice_entryView_createKeyOf(p_entry->key_handle);
        BOOST_SCOPE_EXIT(p_entry_key)
        {
            tetengo_lattice_temp_freeStringView(p_entry_key);
        }
        BOOST_SCOPE_EXIT_END;
        const tetengo::lattice::entry_view cpp_entry{ p_entry_key ?
                                                          std::string_view{ p_entry_key->p_head, p_entry_key->length } :
                                                          std::string_view{},
                                                      reinterpret_cast<const std::any*>(p_entry->value_handle),
                                                      p_entry->cost };
        const std::vector<int>       cpp_preceding_edge_costs{};
        const tetengo::lattice::node cpp_node{
            cpp_entry, preceding_step, &cpp_preceding_edge_costs, best_preceding_node, path_cost
        };

        p_node->key_handle = p_entry->key_handle;
        p_node->value_handle = reinterpret_cast<tetengo_lattice_entryView_valueHandle_t>(&cpp_node.value());
        p_node->preceding_step = cpp_node.preceding_step();
        p_node->p_preceding_edge_costs = p_preceding_edge_costs;
        p_node->preceding_edge_cost_count = preceding_edge_count;
        p_node->best_preceding_node = cpp_node.best_preceding_node();
        p_node->node_cost = cpp_node.node_cost();
        p_node->path_cost = cpp_node.path_cost();

        return 1;
    }
    catch (...)
    {
        return 0;
    }
}

int tetengo_lattice_node_equal(const tetengo_lattice_node_t* const p_one, const tetengo_lattice_node_t* const p_another)
{
    try
    {
        if (!p_one)
        {
            throw std::invalid_argument{ "p_one is NULL." };
        }
        if (!p_another)
        {
            throw std::invalid_argument{ "p_another is NULL." };
        }

        const std::vector<int> cpp_preceding_edge_costs_one{
            p_one->p_preceding_edge_costs, p_one->p_preceding_edge_costs + p_one->preceding_edge_cost_count
        };
        const auto* const p_one_key = tetengo_lattice_entryView_createKeyOf(p_one->key_handle);
        BOOST_SCOPE_EXIT(p_one_key)
        {
            tetengo_lattice_temp_freeStringView(p_one_key);
        }
        BOOST_SCOPE_EXIT_END;
        const tetengo::lattice::node cpp_one{ p_one_key ? std::string_view{ p_one_key->p_head, p_one_key->length } :
                                                          std::string_view{},
                                              reinterpret_cast<const std::any*>(p_one->value_handle),
                                              p_one->preceding_step,
                                              &cpp_preceding_edge_costs_one,
                                              p_one->best_preceding_node,
                                              p_one->node_cost,
                                              p_one->path_cost };
        const std::vector<int> cpp_preceding_edge_costs_another{
            p_another->p_preceding_edge_costs, p_another->p_preceding_edge_costs + p_another->preceding_edge_cost_count
        };
        const auto* const p_another_key = tetengo_lattice_entryView_createKeyOf(p_another->key_handle);
        BOOST_SCOPE_EXIT(p_another_key)
        {
            tetengo_lattice_temp_freeStringView(p_another_key);
        }
        BOOST_SCOPE_EXIT_END;
        const tetengo::lattice::node cpp_another{ p_another_key ?
                                                      std::string_view{ p_another_key->p_head, p_another_key->length } :
                                                      std::string_view{},
                                                  reinterpret_cast<const std::any*>(p_another->value_handle),
                                                  p_another->preceding_step,
                                                  &cpp_preceding_edge_costs_another,
                                                  p_another->best_preceding_node,
                                                  p_another->node_cost,
                                                  p_another->path_cost };

        return cpp_one == cpp_another ? 1 : 0;
    }
    catch (...)
    {
        return 0;
    }
}

int tetengo_lattice_node_isBos(const tetengo_lattice_node_t* const p_node)
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
            tetengo_lattice_temp_freeStringView(p_node_key);
        }
        BOOST_SCOPE_EXIT_END;
        const tetengo::lattice::node cpp_node{ p_node_key ? std::string_view{ p_node_key->p_head, p_node_key->length } :
                                                            std::string_view{},
                                               reinterpret_cast<const std::any*>(p_node->value_handle),
                                               p_node->preceding_step,
                                               &cpp_preceding_edge_costs,
                                               p_node->best_preceding_node,
                                               p_node->node_cost,
                                               p_node->path_cost };
        return cpp_node.is_bos() ? 1 : 0;
    }
    catch (...)
    {
        return 0;
    }
}
