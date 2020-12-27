/*! \file
    \brief A wildcard constraint element.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <cstddef>
#include <limits>
#include <memory>

#include <boost/core/noncopyable.hpp>

#include <tetengo/lattice/node.hpp>
#include <tetengo/lattice/wildcard_constraint_element.hpp>


namespace tetengo::lattice
{
    class wildcard_constraint_element::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        explicit impl(const std::size_t preceding_step) : m_preceding_step{ preceding_step } {}


        // functions

        int matches_impl(const node& node_) const
        {
            if (m_preceding_step == std::numeric_limits<std::size_t>::max())
            {
                if (node_.preceding_step() == std::numeric_limits<std::size_t>::max())
                {
                    return 0;
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                if (node_.preceding_step() < m_preceding_step)
                {
                    return -1;
                }
                else
                {
                    return static_cast<int>(node_.preceding_step() - m_preceding_step);
                }
            }
        }


    private:
        // variables

        const std::size_t m_preceding_step;
    };


    wildcard_constraint_element::wildcard_constraint_element(const std::size_t preceding_step) :
    m_p_impl{ std::make_unique<impl>(preceding_step) }
    {}

    wildcard_constraint_element::~wildcard_constraint_element() = default;

    int wildcard_constraint_element::matches_impl(const node& node_) const
    {
        return m_p_impl->matches_impl(node_);
    }
}
