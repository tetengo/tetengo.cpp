/*! \file
    \brief An entry.

    Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <memory>
#include <utility>

#include <tetengo/lattice/entry.hpp>
#include <tetengo/lattice/input.hpp>


namespace tetengo::lattice
{
    const entry& entry::bos_eos()
    {
        static const entry singleton{ std::unique_ptr<input>{}, std::any{}, 0 };
        return singleton;
    }

    entry::entry(std::unique_ptr<input>&& p_key, std::any value, const int cost) :
    m_p_key{ std::move(p_key) },
    m_value{ std::move(value) },
    m_cost{ cost }
    {}

    entry::entry(const entry_view& view) :
    m_p_key{ view.p_key() ? view.p_key()->clone() : std::unique_ptr<input>{} },
    m_value{ *view.value() },
    m_cost{ view.cost() }
    {}

    entry::entry(const entry& another) :
    m_p_key{ another.m_p_key ? another.m_p_key->clone() : std::unique_ptr<tetengo::lattice::input>{} },
    m_value{ another.m_value },
    m_cost{ another.m_cost }
    {}

    entry::entry(entry&& another) :
    m_p_key{ std::move(another.m_p_key) },
    m_value{ std::move(another.m_value) },
    m_cost{ another.m_cost }
    {}

    const input* entry::p_key() const
    {
        return std::to_address(m_p_key);
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
        static const entry_view singleton{ nullptr, &value_singleton, 0 };
        return singleton;
    }

    entry_view::entry_view(const entry& entry) :
    m_p_key{ entry.p_key() },
    m_value{ &entry.value() },
    m_cost{ entry.cost() }
    {}


}
