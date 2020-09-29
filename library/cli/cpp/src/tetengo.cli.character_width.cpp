/*! \file
    \brief A character width.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <cstddef>
#include <memory>

#include <boost/core/noncopyable.hpp>

#include <tetengo/cli/character_width.hpp>


namespace tetengo::cli
{
    class character_width::impl : private boost::noncopyable
    {
    public:
        // functions

        std::size_t width_of(const class_type class_, const character_width& if_) const
        {
            return if_.width_of_impl(class_);
        }
    };


    character_width::~character_width() = default;

    character_width::character_width() : m_p_impl{ std::make_unique<impl>() } {}

    std::size_t character_width::width_of(const class_type class_) const
    {
        return m_p_impl->width_of(class_, *this);
    }


}
