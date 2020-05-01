/*! \file
    \brief A node.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <stddef.h>

#include <tetengo/lattice/node.h>


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
