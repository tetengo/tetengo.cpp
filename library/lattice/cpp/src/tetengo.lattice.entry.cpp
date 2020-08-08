/*! \file
    \brief An entry.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <cassert> // IWYU pragma: keep
#include <string>
#include <string_view>

#include <tetengo/lattice/entry.hpp>


namespace tetengo::lattice
{
    namespace temp
    {
#if defined(_MSC_VER) && _MSC_VER < 1927
        using empty_type = struct
        {};

        const empty_type empty{};
#endif

        std::any empty_std_any()
        {
#if defined(_MSC_VER) && _MSC_VER < 1927
            return std::any{ &empty };
#else
            return std::any{};
#endif
        }

        bool std_any_has_value(const std::any& any_)
        {
#if defined(_MSC_VER) && _MSC_VER < 1927
            assert(any_.has_value());
            const auto* const* const pp_empty_value = std::any_cast<const empty_type*>(&any_);
            return !pp_empty_value || *pp_empty_value != &empty;
#else
            return any_.has_value();
#endif
        }


    }


    template <>
    const basic_entry<std::string, std::any>& basic_entry<std::string, std::any>::bos_eos()
    {
        static const basic_entry singleton{ std::string{}, temp::empty_std_any(), 0 };
        return singleton;
    }

    template <>
    const basic_entry<std::string_view, const std::any*>& basic_entry<std::string_view, const std::any*>::bos_eos()
    {
        static const std::any    value_singleton = temp::empty_std_any();
        static const basic_entry singleton{ std::string_view{}, &value_singleton, 0 };
        return singleton;
    }

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
