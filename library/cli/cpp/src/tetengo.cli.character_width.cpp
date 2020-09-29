/*! \file
    \brief A character width.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <cstddef>

#include <tetengo/cli/character_width.hpp>


namespace tetengo::cli
{
    character_width::~character_width() = default;

    std::size_t character_width::width_of(const class_type class_) const
    {
        return width_of_impl(class_);
    }


}
