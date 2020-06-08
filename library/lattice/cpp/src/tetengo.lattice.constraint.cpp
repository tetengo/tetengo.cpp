/*! \file
    \brief A constraint.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <cstddef>
#include <limits>
#include <memory>
#include <utility>
#include <vector>

#include <boost/core/noncopyable.hpp>

#include <tetengo/lattice/constraint.hpp>
#include <tetengo/lattice/constraint_element.hpp>
#include <tetengo/lattice/node.hpp> // IWYU pragma: keep


namespace tetengo::lattice
{
    class constraint::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        explicit impl(std::vector<std::unique_ptr<constraint_element>>&& pattern) : m_pattern{ std::move(pattern) } {}


        // functions

        bool matches(const std::vector<node>& path) const
        {
            return matches_impl(path) == 0;
        }

        bool matches_tail(const std::vector<node>& tail_path) const
        {
            return matches_impl(tail_path) != std::numeric_limits<std::size_t>::max();
        }


    private:
        // variables

        const std::vector<std::unique_ptr<constraint_element>> m_pattern;


        // functions

        std::size_t matches_impl(const std::vector<node>& path) const
        {
            auto pattern_index = m_pattern.size();
            for (auto path_index = path.size(); path_index > 0; --path_index)
            {
                const auto element_match = m_pattern[pattern_index - 1]->matches(path[path_index - 1]);
                if (element_match < 0)
                {
                    return std::numeric_limits<std::size_t>::max();
                }
                else if (element_match == 0)
                {
                    if (pattern_index == 0)
                    {
                        return std::numeric_limits<std::size_t>::max();
                    }
                    --pattern_index;
                }
            }

            return pattern_index;
        }
    };


    constraint::constraint(std::vector<std::unique_ptr<constraint_element>>&& pattern) :
    m_p_impl{ std::make_unique<impl>(std::move(pattern)) }
    {}

    constraint::~constraint() = default;

    bool constraint::matches(const std::vector<node>& path) const
    {
        return m_p_impl->matches(path);
    }

    bool constraint::matches_tail(const std::vector<node>& tail_path) const
    {
        return m_p_impl->matches_tail(tail_path);
    }


}
