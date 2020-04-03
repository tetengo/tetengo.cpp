/*! \file
    \brief An entry.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <string_view>
#include <utility>

#include <tetengo/lattice/entry.hpp>


namespace tetengo::lattice
{
    entry::entry(std::string_view key, std::string_view surface, const int cost) :
    m_key{ std::move(key) },
        m_surface{ std::move(surface) },
        m_cost{ cost }
    {}


}
