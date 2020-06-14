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

#include <boost/container_hash/hash.hpp>
#include <boost/operators.hpp>

#include <tetengo/lattice/constraint.hpp>
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


    namespace
    {
        std::size_t calc_node_hash(const node& node_)
        {
            auto seed = static_cast<std::size_t>(0);
            boost::hash_combine(seed, boost::hash_value(node_.key()));
            boost::hash_combine(seed, boost::hash_value(node_.preceding_step()));
            boost::hash_combine(seed, boost::hash_value(node_.preceding_edge_costs()));
            boost::hash_combine(seed, boost::hash_value(node_.best_preceding_node()));
            boost::hash_combine(seed, boost::hash_value(node_.node_cost()));
            boost::hash_combine(seed, boost::hash_value(node_.path_cost()));
            return seed;
        }

        std::vector<node>
        open_cap(const lattice& lattice_, std::priority_queue<cap, std::vector<cap>, std::greater<cap>>& caps)
        {
            const auto opened = caps.top();
            caps.pop();

            auto path = opened.tail_path();
            auto tail_path_cost = opened.tail_path_cost();
            for (const auto* p_node = &opened.tail_path().back(); !p_node->is_bos();)
            {
                const auto& preceding_nodes = lattice_.nodes_at(p_node->preceding_step());
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
                    caps.emplace(std::move(cap_tail_path), cap_tail_path_cost, cap_whole_path_cost);
                }

                const auto  best_preceding_edge_cost = p_node->preceding_edge_costs()[p_node->best_preceding_node()];
                const auto& best_preceding_node = preceding_nodes[p_node->best_preceding_node()];
                path.push_back(best_preceding_node);
                tail_path_cost += best_preceding_edge_cost + best_preceding_node.node_cost();

                p_node = &best_preceding_node;
            }

            std::reverse(std::begin(path), std::end(path));
            return path;
        }


    }

    n_best_iterator::n_best_iterator() :
    m_p_lattice{},
        m_caps{},
        m_eos_hash{ 0 },
        m_p_constraint{ std::make_shared<constraint>() },
        m_path{},
        m_index{ 0 }
    {}

    n_best_iterator::n_best_iterator(
        const lattice&                lattice_,
        node                          eos_node,
        std::unique_ptr<constraint>&& p_constraint) :
    m_p_lattice{ &lattice_ },
        m_caps{},
        m_eos_hash{ calc_node_hash(eos_node) },
        m_p_constraint{ std::move(p_constraint) },
        m_path{},
        m_index{ 0 }
    {
        if (!m_p_constraint)
        {
            throw std::invalid_argument{ "p_constraint is nullptr." };
        }

        const int tail_path_cost = eos_node.node_cost();
        const int whole_path_cost = eos_node.path_cost();
        m_caps.emplace(std::vector<node>{ std::move(eos_node) }, tail_path_cost, whole_path_cost);

        m_path = open_cap(*m_p_lattice, m_caps);
    }

    const std::vector<node>& n_best_iterator::dereference() const
    {
        if (m_path.empty())
        {
            throw std::logic_error{ "No more path." };
        }

        return m_path;
    }

    bool n_best_iterator::equal(const n_best_iterator& another) const
    {
        if (m_path.empty() && another.m_path.empty())
        {
            return true;
        }

        return m_p_lattice == another.m_p_lattice && m_eos_hash == another.m_eos_hash && m_index == another.m_index;
    }

    void n_best_iterator::increment()
    {
        if (m_path.empty())
        {
            throw std::logic_error{ "No more path." };
        }

        if (m_caps.empty())
        {
            m_path.clear();
        }
        else
        {
            m_path = open_cap(*m_p_lattice, m_caps);
        }
        ++m_index;
    }


}
