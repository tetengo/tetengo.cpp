/*! \file
    \brief An N-best lattice path iterator.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <cstddef>
#include <functional>
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
    cap::cap(std::vector<node> tail, const int whole_path_cost) :
    m_tail{ std::move(tail) },
        m_whole_path_cost{ whole_path_cost }
    {}

    bool operator<(const cap& one, const cap& another)
    {
        return one.m_whole_path_cost < another.m_whole_path_cost;
    }

    const std::vector<node>& cap::tail() const
    {
        return m_tail;
    }


    n_best_iterator::n_best_iterator() : m_p_lattice{}, m_caps{} {}

    n_best_iterator::n_best_iterator(const lattice& lattice_, node eos_node) : m_p_lattice{ &lattice_ }, m_caps{}
    {
        const int whole_path_cost = eos_node.path_cost();
        m_caps.emplace(std::vector<node>{ std::move(eos_node) }, whole_path_cost);
    }

    std::vector<node> n_best_iterator::dereference() const
    {
        if (m_caps.empty())
        {
            throw std::logic_error{ "No more path." };
        }

        auto path = m_caps.top().tail();
        for (const auto* p_node = &m_caps.top().tail().back(); !p_node->is_bos();)
        {
            const auto&       preceding_nodes = m_p_lattice->nodes_at(p_node->preceding_step());
            const auto* const p_best_preceding_node = &preceding_nodes[p_node->best_preceding_node()];
            path.push_back(*p_best_preceding_node);

            p_node = p_best_preceding_node;
        }

        return path;
    }

    void n_best_iterator::increment()
    {
        if (m_caps.empty())
        {
            throw std::logic_error{ "No more path." };
        }

        const cap opened = m_caps.top();
        m_caps.pop();

        auto path = opened.tail();
        for (const auto* p_node = &opened.tail().back(); !p_node->is_bos();)
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
                m_caps.emplace(std::move(cap_tail_path), preceding_node.path_cost());
            }

            const auto& best_preceding_node = preceding_nodes[p_node->best_preceding_node()];
            path.push_back(best_preceding_node);
            p_node = &best_preceding_node;
        }
    }


}
