/*! \file
    \brief A character width.

    Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/
*/

#include <cstddef>

#include <tetengo/text/grapheme_splitting/character_width.hpp>


namespace tetengo::text::grapheme_splitting
{
    character_width::~character_width() = default;

    std::size_t character_width::width_of(const class_type class_, const emoji_type emoji) const
    {
        return width_of_impl(class_, emoji);
    }


}
