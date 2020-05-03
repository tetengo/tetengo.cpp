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


const tetengo_lattice_node* tetengo_lattice_node_bos()
{
    try
    {
        static const tetengo_lattice_node singleton{ { nullptr, 0 }, nullptr, 0, 0, 0 };
        return &singleton;
    }
    catch (...)
    {
        return nullptr;
    }
}

int tetengo_lattice_node_eos(const size_t preceding, const int path_cost, tetengo_lattice_node* const p_eos)
{
    try
    {
        if (!p_eos)
        {
            return 0;
        }

        *p_eos = *tetengo_lattice_node_bos();
        p_eos->preceding = preceding;
        p_eos->path_cost = path_cost;

        return 1;
    }
    catch (...)
    {
        return 0;
    }
}

int tetengo_lattice_node_toNode(
    const tetengo_lattice_entry* p_entry,
    size_t                       preceding,
    int                          path_cost,
    tetengo_lattice_node*        p_node)
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

        const std::any                     cpp_entry_value{ p_entry->p_value };
        const tetengo::lattice::entry_view cpp_entry{ std::string_view{ p_entry->key.p_head, p_entry->key.length },
                                                      &cpp_entry_value,
                                                      p_entry->cost };
        const tetengo::lattice::node       cpp_node{ cpp_entry, preceding, path_cost };

        p_node->key.p_head = cpp_node.key().data();
        p_node->key.length = cpp_node.key().length();
        p_node->p_value = std::any_cast<const void*>(cpp_node.value());
        p_node->preceding = cpp_node.preceding();
        p_node->node_cost = cpp_node.node_cost();
        p_node->path_cost = cpp_node.path_cost();

        return 1;
    }
    catch (...)
    {
        return 0;
    }
}