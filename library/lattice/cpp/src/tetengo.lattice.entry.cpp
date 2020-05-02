/*! \file
    \brief An entry.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <string>
#include <string_view>

#include <tetengo/lattice/entry.hpp>


namespace tetengo::lattice
{
    template <typename Key, typename Value>
    basic_entry<Key, Value>::basic_entry(key_type key, value_type value, const int cost) :
    m_key{ std::move(key) },
        m_value{ std::move(value) },
        m_cost{ cost }
    {}

    template <>
    template <>
    basic_entry<std::string_view, const std::any*>::basic_entry(const basic_entry<std::string, std::any>& another) :
    m_key{ another.key() },
        m_value{ &another.value() },
        m_cost{ another.cost() }
    {}

    template <>
    template <>
    basic_entry<std::string, std::any>::basic_entry(const basic_entry<std::string_view, const std::any*>& another) :
    m_key{ another.key() },
        m_value{ *another.value() },
        m_cost{ another.cost() }
    {}

    template <typename Key, typename Value>
    const typename basic_entry<Key, Value>::key_type& basic_entry<Key, Value>::key() const
    {
        return m_key;
    }

    template <typename Key, typename Value>
    const typename basic_entry<Key, Value>::value_type& basic_entry<Key, Value>::value() const
    {
        return m_value;
    }

    template <typename Key, typename Value>
    int basic_entry<Key, Value>::cost() const
    {
        return m_cost;
    }


    template class basic_entry<std::string, std::any>;

    template class basic_entry<std::string_view, const std::any*>;


}
