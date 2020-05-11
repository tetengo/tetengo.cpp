/*! \file
    \brief An N-best lattice path iterator.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <memory>
#include <queue>
#include <stdexcept>
#include <utility>
#include <vector>

#include <boost/operators.hpp>

#include <tetengo/lattice/lattice.hpp>
#include <tetengo/lattice/n_best_iterator.hpp>
#include <tetengo/lattice/node.hpp>


namespace tetengo::lattice
{
    cap::cap(std::vector<node> tail_path, const int tail_path_cost, const int whole_path_cost) :
    m_tail_path{ std::move(tail_path) },
        m_tail_path_cost{ tail_path_cost },
        m_whole_path_cost{ whole_path_cost }
    {}

    bool operator<(const cap& one, const cap& another)
    {
        return one.m_whole_path_cost < another.m_whole_path_cost;
    }

    const std::vector<node>& cap::tail_path() const
    {
        return m_tail_path;
    }

    int cap::tail_path_cost() const
    {
        return m_tail_path_cost;
    }

    int cap::whole_path_cost() const
    {
        return m_whole_path_cost;
    }


    n_best_iterator::n_best_iterator() : m_p_lattice{}, m_caps{}, m_index{ 0 } {}

    n_best_iterator::n_best_iterator(const lattice& lattice_, node eos_node) :
    m_p_lattice{ &lattice_ },
        m_caps{},
        m_index{ 0 }
    {
        const int tail_path_cost = eos_node.node_cost();
        const int whole_path_cost = eos_node.path_cost();
        m_caps.emplace(std::vector<node>{ std::move(eos_node) }, tail_path_cost, whole_path_cost);
    }

    std::vector<node> n_best_iterator::dereference() const
    {
        if (m_caps.empty())
        {
            throw std::logic_error{ "No more path." };
        }

        auto path = m_caps.top().tail_path();
        for (const auto* p_node = &m_caps.top().tail_path().back(); !p_node->is_bos();)
        {
            const auto& preceding_nodes = m_p_lattice->nodes_at(p_node->preceding_step());
            const auto& best_preceding_node = preceding_nodes[p_node->best_preceding_node()];
            path.push_back(best_preceding_node);

            p_node = &best_preceding_node;
        }

        std::reverse(std::begin(path), std::end(path));
        return path;
    }

    bool n_best_iterator::equal(const n_best_iterator& another) const
    {
        if (m_caps.empty() && another.m_caps.empty())
        {
            return true;
        }

        return m_p_lattice == another.m_p_lattice && m_index == another.m_index;
    }

    void n_best_iterator::increment()
    {
        if (m_caps.empty())
        {
            throw std::logic_error{ "No more path." };
        }

        const cap opened = m_caps.top();
        m_caps.pop();

        auto path = opened.tail_path();
        auto tail_path_cost = opened.tail_path_cost();
        for (const auto* p_node = &opened.tail_path().back(); !p_node->is_bos();)
        {
            const auto& preceding_nodes = m_p_lattice->nodes_at(p_node->preceding_step());
            for (auto i = static_cast<std::size_t>(0); i < preceding_nodes.size(); ++i)
            {
                if (i == p_node->best_preceding_node())
                {
                    continue;
                }
                const auto&       preceding_node = preceding_nodes[i];
                std::vector<node> cap_tail_path{ path };
                cap_tail_path.push_back(preceding_node);
                const auto preceding_edge_cost = p_node->preceding_edge_costs()[i];
                const auto cap_tail_path_cost = tail_path_cost + preceding_edge_cost + preceding_node.node_cost();
                const auto cap_whole_path_cost = tail_path_cost + preceding_edge_cost + preceding_node.path_cost();
                m_caps.emplace(std::move(cap_tail_path), cap_tail_path_cost, cap_whole_path_cost);
            }

            const auto  best_preceding_edge_cost = p_node->preceding_edge_costs()[p_node->best_preceding_node()];
            const auto& best_preceding_node = preceding_nodes[p_node->best_preceding_node()];
            path.push_back(best_preceding_node);
            tail_path_cost += best_preceding_edge_cost + best_preceding_node.node_cost();
            p_node = &best_preceding_node;
        }

        ++m_index;
    }


}
