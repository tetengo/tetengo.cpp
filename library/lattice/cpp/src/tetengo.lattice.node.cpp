/*! \file
    \brief A node.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <string_view>
#include <utility>

#include <tetengo/lattice/node.hpp>


namespace tetengo::lattice
{
    node::node(std::string_view key, std::any value, int node_cost, int path_cost) :
    m_key{ std::move(key) },
        m_value{ std::move(value) },
        m_node_cost{ node_cost },
        m_path_cost{ path_cost }
    {}

    const std::string_view& node::key() const
    {
        return m_key;
    }

    const std::any& node::value() const
    {
        return m_value;
    }

    int node::node_cost() const
    {
        return m_node_cost;
    }

    int node::path_cost() const
    {
        return m_path_cost;
    }


}
