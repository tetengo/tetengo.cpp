/*! \file
    \brief A lattice.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <boost/core/noncopyable.hpp>

#include <tetengo/lattice/connection.hpp>
#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/lattice.hpp>
#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/vocabulary.hpp>


namespace tetengo::lattice
{
    class graph_step
    {
    public:
        // constructors and destructor

        graph_step(const std::size_t input_tail, std::vector<node> nodes) :
        m_input_tail{ input_tail },
            m_nodes{ std::move(nodes) }
        {}


        // functions

        std::size_t input_tail() const
        {
            return m_input_tail;
        }

        const std::vector<node>& nodes() const
        {
            return m_nodes;
        }


    private:
        // variables

        std::size_t m_input_tail;

        std::vector<node> m_nodes;
    };


    class lattice::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        explicit impl(std::unique_ptr<vocabulary>&& p_vocabulary) :
        m_p_vocabulary{ std::move(p_vocabulary) },
            m_input{},
            m_graph{ { bos() } }
        {}


        // functions

        std::size_t step_count() const
        {
            return m_graph.size();
        }

        const std::vector<node>& nodes_at(const std::size_t step) const
        {
            if (step >= m_graph.size())
            {
                throw std::out_of_range{ "step is too large." };
            }

            return m_graph[step].nodes();
        }

        void push_back(const std::string_view& input)
        {
            m_input += input;

            std::vector<node> nodes{};
            for (auto i = static_cast<std::size_t>(0); i < m_graph.size(); ++i)
            {
                const auto& step = m_graph[i];

                const std::string_view node_key{ std::next(m_input.data(), step.input_tail()),
                                                 m_input.length() - step.input_tail() };
                const auto             found = m_p_vocabulary->find_entries(node_key);

                std::transform(
                    std::begin(found), std::end(found), std::back_inserter(nodes), [this, i, &step](const auto& e) {
                        const auto preceding_edge_costs_ = preceding_edge_costs(step, e);
                        const auto best_preceding_node_index_ = best_preceding_node_index(step, preceding_edge_costs_);
                        const auto best_preceding_path_cost = step.nodes()[best_preceding_node_index_].path_cost() +
                                                              preceding_edge_costs_[best_preceding_node_index_];
                        return node{ e, i, best_preceding_node_index_, best_preceding_path_cost + e.cost() };
                    });
            }
            m_graph.emplace_back(m_input.length(), std::move(nodes));
        }

        node settle()
        {
            const auto preceding_edge_costs_ = preceding_edge_costs(m_graph.back(), entry_view::bos_eos());
            const auto best_preceding_node_index_ = best_preceding_node_index(m_graph.back(), preceding_edge_costs_);
            const auto best_preceding_path_cost = m_graph.back().nodes()[best_preceding_node_index_].path_cost() +
                                                  preceding_edge_costs_[best_preceding_node_index_];
            return node::eos(m_graph.size() - 1, best_preceding_node_index_, best_preceding_path_cost);
        }


    private:
        // static functions

        static const graph_step& bos()
        {
            static const graph_step singleton{ 0, std::vector<node>{ node::bos() } };
            return singleton;
        }

        static std::size_t best_preceding_node_index(const graph_step& step, const std::vector<int>& edge_costs)
        {
            assert(!step.nodes().empty());
            auto min_index = static_cast<std::size_t>(0);
            for (auto i = static_cast<std::size_t>(1); i < step.nodes().size(); ++i)
            {
                if (add_cost(step.nodes()[i].path_cost(), edge_costs[i]) <
                    add_cost(step.nodes()[min_index].path_cost(), edge_costs[min_index]))
                {
                    min_index = i;
                }
            }
            return min_index;
        }

        static int add_cost(const int one, const int another)
        {
            if (one == std::numeric_limits<int>::max() || another == std::numeric_limits<int>::max())
            {
                return std::numeric_limits<int>::max();
            }
            else
            {
                return one + another;
            }
        }


        // variables

        const std::unique_ptr<vocabulary> m_p_vocabulary;

        std::string m_input;

        std::vector<graph_step> m_graph;


        // functions

        std::vector<int> preceding_edge_costs(const graph_step& step, const entry_view& next_entry) const
        {
            assert(!step.nodes().empty());
            std::vector<int> costs{};
            costs.reserve(step.nodes().size());
            std::transform(
                std::begin(step.nodes()),
                std::end(step.nodes()),
                std::back_inserter(costs),
                [this, &next_entry](const auto& node) {
                    return m_p_vocabulary->find_connection(node, next_entry).cost();
                });
            return costs;
        }
    };


    lattice::lattice(std::unique_ptr<vocabulary>&& p_vocabulary) :
    m_p_impl{ std::make_unique<impl>(std::move(p_vocabulary)) }
    {}

    lattice::~lattice() = default;

    std::size_t lattice::step_count() const
    {
        return m_p_impl->step_count();
    }

    const std::vector<node>& lattice::nodes_at(const std::size_t step) const
    {
        return m_p_impl->nodes_at(step);
    }

    void lattice::push_back(const std::string_view& input)
    {
        m_p_impl->push_back(input);
    }

    node lattice::settle()
    {
        return m_p_impl->settle();
    }


}
