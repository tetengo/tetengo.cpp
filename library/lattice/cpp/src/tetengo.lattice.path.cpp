/*! \file
    \brief A path.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <cstddef>
#include <utility>
#include <vector>

#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/path.hpp>


namespace tetengo::lattice
{
    path::path(std::vector<node> nodes, const std::size_t cost) : m_nodes{ std::move(nodes) }, m_cost{ cost } {}

    const std::vector<node>& path::nodes() const
    {
        return m_nodes;
    }

    std::size_t path::cost() const
    {
        return m_cost;
    }


}
