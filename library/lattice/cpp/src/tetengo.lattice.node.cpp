/*! \file
    \brief A node.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <cstddef>
#include <string_view>
#include <utility>

#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/node.hpp>


namespace tetengo::lattice
{
    const node& node::bos()
    {
        static const node& singleton{ entry_view::bos_eos(), 0, 0 };
        return singleton;
    }

    node node::eos(const std::size_t preceding, const int path_cost)
    {
        return node{ entry_view::bos_eos(), preceding, path_cost };
    }

    node::node(
        std::string_view  key,
        std::any          value,
        const std::size_t preceding,
        const int         node_cost,
        const int         path_cost) :
    m_key{ std::move(key) },
        m_value{ std::move(value) },
        m_preceding{ preceding },
        m_node_cost{ node_cost },
        m_path_cost{ path_cost }
    {}

    node::node(
        const entry_view& entry,
        const std::size_t preceding /*= std::numeric_limits<std::size_t>::max()*/,
        const int         path_cost /*= std::numeric_limits<int>::max()*/) :
    node{ entry.key(), entry.value() ? *entry.value() : std::any{}, preceding, entry.cost(), path_cost }
    {}

    const std::string_view& node::key() const
    {
        return m_key;
    }

    const std::any& node::value() const
    {
        return m_value;
    }

    std::size_t node::preceding() const
    {
        return m_preceding;
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
