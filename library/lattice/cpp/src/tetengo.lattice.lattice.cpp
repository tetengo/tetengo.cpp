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
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <boost/core/noncopyable.hpp>

#include <tetengo/lattice/connection.hpp>
#include <tetengo/lattice/lattice.hpp>
#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/vocabulary.hpp>


namespace tetengo::lattice
{
    class graph_step
    {
    public:
        // constructors and destructor

        explicit graph_step(const std::size_t input_tail, std::vector<node> nodes) :
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
                        const auto lowest_preceding_path_cost = lowest_path_cost(step, node{ e });

                        return node{ e, i, lowest_preceding_path_cost + e.cost() };
                    });
            }
            m_graph.emplace_back(m_input.length(), std::move(nodes));
        }


    private:
        // static functions

        static const graph_step& bos()
        {
            static const graph_step singleton{ 0, std::vector<node>{ node::bos() } };
            return singleton;
        }

        static int add_connection_cost(const int one, const int another)
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

        int lowest_path_cost(const graph_step& step, const node& next_node) const
        {
            assert(!step.nodes().empty());
            const auto i_min_cost_node = std::min_element(
                std::begin(step.nodes()),
                std::end(step.nodes()),
                [this, &next_node](const auto& one, const auto& another) {
                    return add_connection_cost(
                               one.path_cost(), m_p_vocabulary->find_connection(one, next_node).cost()) <
                           add_connection_cost(
                               another.path_cost(), m_p_vocabulary->find_connection(another, next_node).cost());
                });

            return add_connection_cost(
                i_min_cost_node->path_cost(), m_p_vocabulary->find_connection(*i_min_cost_node, next_node).cost());
        }
    };


    lattice::lattice(std::unique_ptr<vocabulary>&& p_vocabulary) :
    m_p_impl{ std::make_unique<impl>(std::move(p_vocabulary)) }
    {}

    lattice::~lattice() = default;

    void lattice::push_back(const std::string_view& input)
    {
        m_p_impl->push_back(input);
    }
}
