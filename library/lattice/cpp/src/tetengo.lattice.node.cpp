/*! \file
    \brief A node.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <vector>

#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/node.hpp>


namespace tetengo::lattice
{
    const node& node::bos()
    {
        static const node singleton{ entry_view::bos_eos(),
                                     std::numeric_limits<std::size_t>::max(),
                                     std::vector<int>{},
                                     std::numeric_limits<std::size_t>::max(),
                                     0 };
        return singleton;
    }

    node node::eos(
        const std::size_t preceding_step,
        std::vector<int>  preceding_edge_costs,
        const std::size_t best_preceding_node,
        const int         path_cost)
    {
        return node{
            entry_view::bos_eos(), preceding_step, std::move(preceding_edge_costs), best_preceding_node, path_cost
        };
    }

    node::node(
        std::string_view  key,
        const std::any*   p_value,
        const std::size_t preceding_step,
        std::vector<int>  preceding_edge_costs,
        const std::size_t best_preceding_node,
        const int         node_cost,
        const int         path_cost) :
    m_key{ std::move(key) },
        m_p_value{ p_value },
        m_preceding_step{ preceding_step },
        m_preceding_edge_costs{ std::move(preceding_edge_costs) },
        m_best_preceding_node{ best_preceding_node },
        m_node_cost{ node_cost },
        m_path_cost{ path_cost }
    {
        if (!p_value)
        {
            throw std::invalid_argument{ "p_value is nullptr." };
        }
    }

    node::node(
        const entry_view& entry,
        const std::size_t preceding_step,
        std::vector<int>  preceding_edge_costs,
        const std::size_t best_preceding_node,
        const int         path_cost) :
    node{ entry.key(),         entry.value(), preceding_step, std::move(preceding_edge_costs),
          best_preceding_node, entry.cost(),  path_cost }
    {}

    const std::string_view& node::key() const
    {
        return m_key;
    }

    const std::any& node::value() const
    {
        return *m_p_value;
    }

    std::size_t node::preceding_step() const
    {
        return m_preceding_step;
    }

    const std::vector<int>& node::preceding_edge_costs() const
    {
        return m_preceding_edge_costs;
    }

    std::size_t node::best_preceding_node() const
    {
        return m_best_preceding_node;
    }

    int node::node_cost() const
    {
        return m_node_cost;
    }

    int node::path_cost() const
    {
        return m_path_cost;
    }

    bool node::is_bos() const
    {
        return key() == bos().key() && preceding_step() == bos().preceding_step() &&
               best_preceding_node() == bos().best_preceding_node() && node_cost() == bos().node_cost() &&
               path_cost() == bos().path_cost();
    }


}
