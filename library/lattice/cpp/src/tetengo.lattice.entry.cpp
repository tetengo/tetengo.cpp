/*! \file
    \brief An entry.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <cassert> // IWYU pragma: keep
#include <string>
#include <string_view>
#include <utility>

#include <tetengo/lattice/entry.hpp>


namespace tetengo::lattice
{
    const entry& entry::bos_eos()
    {
        static const entry singleton{ std::string{}, std::any{}, 0 };
        return singleton;
    }

    entry::entry(std::string key, std::any value, const int cost) :
    m_key{ std::move(key) },
    m_value{ std::move(value) },
    m_cost{ cost }
    {}

    entry::entry(const entry_view& view) : m_key{ view.key() }, m_value{ *view.value() }, m_cost{ view.cost() } {}

    const std::string& entry::key() const
    {
        return m_key;
    }

    const std::any& entry::value() const
    {
        return m_value;
    }

    int entry::cost() const
    {
        return m_cost;
    }


    const entry_view& entry_view::bos_eos()
    {
        static const std::any   value_singleton = std::any{};
        static const entry_view singleton{ std::string_view{}, &value_singleton, 0 };
        return singleton;
    }

    entry_view::entry_view(const entry& entry) : m_key{ entry.key() }, m_value{ &entry.value() }, m_cost{ entry.cost() }
    {}


}
