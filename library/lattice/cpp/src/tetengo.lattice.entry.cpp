/*! \file
    \brief An entry.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <string>
#include <string_view>

#include <tetengo/lattice/entry.hpp>


namespace tetengo::lattice
{
    template <typename String>
    basic_entry<String>::basic_entry(string_type key, string_type surface, const int cost) :
    m_key{ std::move(key) },
        m_surface{ std::move(surface) },
        m_cost{ cost }
    {}

    template <typename String>
    const typename basic_entry<String>::string_type& basic_entry<String>::key() const
    {
        return m_key;
    }

    template <typename String>
    const typename basic_entry<String>::string_type& basic_entry<String>::surface() const
    {
        return m_surface;
    }

    template <typename String>
    int basic_entry<String>::cost() const
    {
        return m_cost;
    }


    template class basic_entry<std::string>;

    template class basic_entry<std::string_view>;


}
