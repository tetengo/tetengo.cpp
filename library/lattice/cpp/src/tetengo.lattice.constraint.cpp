/*! \file
    \brief A constraint.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <memory>
#include <utility>
#include <vector>

#include <boost/core/noncopyable.hpp>

#include <tetengo/lattice/constraint.hpp>
#include <tetengo/lattice/constraint_element.hpp>


namespace tetengo::lattice
{
    class constraint::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        explicit impl(std::vector<std::unique_ptr<constraint_element>>&& pattern) : m_pattern{ std::move(pattern) } {}


    private:
        // variables

        const std::vector<std::unique_ptr<constraint_element>> m_pattern;
    };


    constraint ::constraint(std::vector<std::unique_ptr<constraint_element>>&& pattern) :
    m_p_impl{ std::make_unique<impl>(std::move(pattern)) }
    {}

    constraint ::~constraint() = default;


}
