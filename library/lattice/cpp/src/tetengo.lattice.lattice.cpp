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
#include <type_traits> // IWYU pragma: keep
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

        graph_step(
            const std::size_t                                input_tail,
            std::vector<node>                                nodes,
            std::vector<std::unique_ptr<std::vector<int>>>&& p_preceding_edge_costs) :
        m_input_tail{ input_tail },
            m_nodes{ std::move(nodes) },
            m_p_preceding_edge_costs{ std::move(p_preceding_edge_costs) }
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

        const std::vector<int>& preceding_edge_costs(const std::size_t index) const
        {
            assert(index < std::size(m_p_preceding_edge_costs));
            assert(m_p_preceding_edge_costs[index]);
            return *m_p_preceding_edge_costs[index];
        }


    private:
        // variables

        std::size_t m_input_tail;

        std::vector<node> m_nodes;

        std::vector<std::unique_ptr<std::vector<int>>> m_p_preceding_edge_costs;
    };


    class lattice::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        explicit impl(std::unique_ptr<vocabulary>&& p_vocabulary) :
        m_p_vocabulary{ std::move(p_vocabulary) },
            m_input{},
            m_graph{}
        {
            m_graph.push_back(bos_step());
        }


        // functions

        std::size_t step_count() const
        {
            return std::size(m_graph);
        }

        const std::vector<node>& nodes_at(const std::size_t step) const
        {
            if (step >= std::size(m_graph))
            {
                throw std::out_of_range{ "step is too large." };
            }

            return m_graph[step].nodes();
        }

        void push_back(const std::string_view& input)
        {
            m_input += input;

            std::vector<node> nodes{};
            auto              p_node_preceding_edge_costs = std::vector<std::unique_ptr<std::vector<int>>>{};
            for (auto i = static_cast<std::size_t>(0); i < std::size(m_graph); ++i)
            {
                const auto& step = m_graph[i];

                const std::string_view node_key{ std::next(m_input.data(), step.input_tail()),
                                                 m_input.length() - step.input_tail() };
                const auto             found = m_p_vocabulary->find_entries(node_key);

                std::vector<std::size_t> preceding_edge_cost_indexes{};
                preceding_edge_cost_indexes.reserve(std::size(found));
                for (const auto& e: found)
                {
                    auto p_preceding_edge_costs = preceding_edge_costs(step, e);
                    preceding_edge_cost_indexes.push_back(std::size(p_node_preceding_edge_costs));
                    p_node_preceding_edge_costs.push_back(std::move(p_preceding_edge_costs));
                }

                for (auto j = static_cast<std::size_t>(0); j < std::size(found); ++j)
                {
                    const auto& entry = found[j];
                    const auto& preceding_edge_costs = *p_node_preceding_edge_costs[preceding_edge_cost_indexes[j]];

                    const auto best_preceding_node_index_ = best_preceding_node_index(step, preceding_edge_costs);
                    const auto best_preceding_path_cost = add_cost(
                        step.nodes()[best_preceding_node_index_].path_cost(),
                        preceding_edge_costs[best_preceding_node_index_]);

                    nodes.emplace_back(
                        entry,
                        i,
                        &preceding_edge_costs,
                        best_preceding_node_index_,
                        add_cost(best_preceding_path_cost, entry.cost()));
                }
            }
            if (nodes.empty())
            {
                throw std::invalid_argument{ "No node is found for the input." };
            }

            m_graph.emplace_back(m_input.length(), std::move(nodes), std::move(p_node_preceding_edge_costs));
        }

        std::pair<node, std::unique_ptr<std::vector<int>>> settle()
        {
            auto       p_preceding_edge_costs = preceding_edge_costs(m_graph.back(), entry_view::bos_eos());
            const auto best_preceding_node_index_ = best_preceding_node_index(m_graph.back(), *p_preceding_edge_costs);
            const auto best_preceding_path_cost = add_cost(
                m_graph.back().nodes()[best_preceding_node_index_].path_cost(),
                (*p_preceding_edge_costs)[best_preceding_node_index_]);

            node eos_node{ node::eos(
                std::size(m_graph) - 1,
                p_preceding_edge_costs.get(),
                best_preceding_node_index_,
                best_preceding_path_cost) };
            return std::make_pair(std::move(eos_node), std::move(p_preceding_edge_costs));
        }


    private:
        // static functions

        static graph_step bos_step()
        {
            std::vector<std::unique_ptr<std::vector<int>>> p_node_preceding_edge_costs{};
            p_node_preceding_edge_costs.push_back(std::make_unique<std::vector<int>>());
            std::vector<node> nodes{ node::bos(p_node_preceding_edge_costs[0].get()) };
            return graph_step{ 0, std::move(nodes), std::move(p_node_preceding_edge_costs) };
        }

        static std::size_t best_preceding_node_index(const graph_step& step, const std::vector<int>& edge_costs)
        {
            assert(!step.nodes().empty());
            auto min_index = static_cast<std::size_t>(0);
            for (auto i = static_cast<std::size_t>(1); i < std::size(step.nodes()); ++i)
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

        std::unique_ptr<std::vector<int>>
        preceding_edge_costs(const graph_step& step, const entry_view& next_entry) const
        {
            assert(!step.nodes().empty());
            std::vector<int> costs{};
            costs.reserve(std::size(step.nodes()));
            std::transform(
                std::begin(step.nodes()),
                std::end(step.nodes()),
                std::back_inserter(costs),
                [this, &next_entry](const auto& node) {
                    return m_p_vocabulary->find_connection(node, next_entry).cost();
                });
            return std::make_unique<std::vector<int>>(std::move(costs));
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

    std::pair<node, std::unique_ptr<std::vector<int>>> lattice::settle()
    {
        return m_p_impl->settle();
    }
}
