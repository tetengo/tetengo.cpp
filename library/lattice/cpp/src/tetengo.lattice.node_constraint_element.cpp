/*! \file
    \brief A node constraint element.

    Copyright (C) 2019-2026 kaoru  https://www.tetengo.org/
*/

#include <memory>
#include <utility>

#include <boost/core/noncopyable.hpp>

#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/node_constraint_element.hpp>


namespace tetengo::lattice
{
    class node_constraint_element::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        explicit impl(node node_) : m_node{ std::move(node_) } {}


        // functions

        int matches_impl(const node& node_) const
        {
            return node_ == m_node ? 0 : -1;
        }


    private:
        // variables

        const node m_node;
    };


    node_constraint_element::node_constraint_element(node node_) : m_p_impl{ std::make_unique<impl>(std::move(node_)) }
    {}

    node_constraint_element::~node_constraint_element() = default;

    int node_constraint_element::matches_impl(const node& node_) const
    {
        return m_p_impl->matches_impl(node_);
    }


}
