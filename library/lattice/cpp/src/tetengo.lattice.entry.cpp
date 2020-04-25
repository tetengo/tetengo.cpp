/*! \file
    \brief An entry.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <string>
#include <string_view>

#include <tetengo/lattice/entry.hpp>


namespace tetengo::lattice
{
    template <typename Key>
    basic_entry<Key>::basic_entry(key_type key, key_type surface, const int cost) :
    m_key{ std::move(key) },
        m_surface{ std::move(surface) },
        m_cost{ cost }
    {}

    template <typename Key>
    template <typename K>
    basic_entry<Key>::basic_entry(const basic_entry<K>& another) :
    m_key{ another.key() },
        m_surface{ another.surface() },
        m_cost{ another.cost() }
    {}

    template <typename Key>
    const typename basic_entry<Key>::key_type& basic_entry<Key>::key() const
    {
        return m_key;
    }

    template <typename Key>
    const typename basic_entry<Key>::key_type& basic_entry<Key>::surface() const
    {
        return m_surface;
    }

    template <typename Key>
    int basic_entry<Key>::cost() const
    {
        return m_cost;
    }


    template class basic_entry<std::string>;

    template class basic_entry<std::string_view>;

    template basic_entry<std::string_view>::basic_entry(const basic_entry<std::string>&);


}
