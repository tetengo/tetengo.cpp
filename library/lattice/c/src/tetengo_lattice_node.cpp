/*! \file
    \brief A node.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <string_view>

#include <stddef.h>

#include <tetengo/lattice/entry.h> // IWYU pragma: keep
#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/node.h>
#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/string_view.h>


const tetengo_lattice_node_t* tetengo_lattice_node_bos()
{
    try
    {
        static const tetengo_lattice_node_t singleton{
            { tetengo::lattice::node::bos().key().data(), tetengo::lattice::node::bos().key().length() },
            reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&tetengo::lattice::node::bos().value()),
            tetengo::lattice::node::bos().preceding_step(),
            tetengo::lattice::node::bos().best_preceding_node(),
            tetengo::lattice::node::bos().node_cost(),
            tetengo::lattice::node::bos().path_cost()
        };
        return &singleton;
    }
    catch (...)
    {
        return nullptr;
    }
}

int tetengo_lattice_node_eos(
    const size_t                  preceding_step,
    const size_t                  best_preceding_node,
    const int                     path_cost,
    tetengo_lattice_node_t* const p_eos)
{
    try
    {
        if (!p_eos)
        {
            return 0;
        }

        const auto cpp_eos = tetengo::lattice::node::eos(preceding_step, best_preceding_node, path_cost);

        p_eos->key.p_head = cpp_eos.key().data();
        p_eos->key.length = cpp_eos.key().length();
        p_eos->value_handle = reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&cpp_eos.value());
        p_eos->preceding_step = cpp_eos.preceding_step();
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
    const size_t                             best_preceding_node,
    const int                                path_cost,
    tetengo_lattice_node_t* const            p_node)
{
    try
    {
        if (!p_entry)
        {
            return 0;
        }
        if (!p_node)
        {
            return 0;
        }

        const tetengo::lattice::entry_view cpp_entry{ std::string_view{ p_entry->key.p_head, p_entry->key.length },
                                                      reinterpret_cast<const std::any*>(p_entry->value_handle),
                                                      p_entry->cost };
        const tetengo::lattice::node       cpp_node{ cpp_entry, preceding_step, best_preceding_node, path_cost };

        p_node->key.p_head = cpp_node.key().data();
        p_node->key.length = cpp_node.key().length();
        p_node->value_handle = reinterpret_cast<tetengo_lattice_entry_valueHandle_t>(&cpp_node.value());
        p_node->preceding_step = cpp_node.preceding_step();
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
