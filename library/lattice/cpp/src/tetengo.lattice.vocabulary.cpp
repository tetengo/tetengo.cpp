/*! \file
    \brief A vocabulary.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <string_view>
#include <vector>

#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/vocabulary.hpp>


namespace tetengo::lattice
{
    vocabulary::vocabulary() = default;

    vocabulary::~vocabulary() = default;

    std::vector<entry> vocabulary::find(const std::string_view& key) const
    {
        return find_impl(key);
    }


}
