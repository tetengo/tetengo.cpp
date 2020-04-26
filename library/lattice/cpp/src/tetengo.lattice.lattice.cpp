/*! \file
    \brief A lattice.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <boost/core/noncopyable.hpp>

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

        const std::vector<node> nodes() const
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
            m_graph{}
        {}


        // functions

        void push_back(const std::string_view& input)
        {
            m_input += input;
            m_graph.emplace_back(m_input.length(), std::vector<node>{});
        }


    private:
        // variables

        const std::unique_ptr<vocabulary> m_p_vocabulary;

        std::string m_input;

        std::vector<graph_step> m_graph;
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
