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
    path::path() : m_nodes{}, m_cost{ 0 } {}

    path::path(std::vector<node> nodes, const std::size_t cost) : m_nodes{ std::move(nodes) }, m_cost{ cost } {}

    bool path::empty() const
    {
        return m_nodes.empty();
    }

    const std::vector<node>& path::nodes() const
    {
        return m_nodes;
    }

    std::size_t path::cost() const
    {
        return m_cost;
    }


}
