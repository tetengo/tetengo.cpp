/*! \file
    \brief A storage.

    Copyright (C) 2019 kaoru
*/

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include <tetengo/trie/storage.hpp>


namespace tetengo::trie
{
    namespace
    {
        void ensure_size(std::vector<std::pair<std::int32_t, std::uint8_t>>& values, const std::size_t size)
        {
            if (size > values.size())
            {
                values.resize(size);
            }
        }


    }


    storage::storage() : m_values{} {}

    std::int32_t storage::base_at(const std::size_t index) const
    {
        ensure_size(m_values, index + 1);
        return m_values[index].first;
    }

    void storage::set_base_at(const std::size_t index, const std::int32_t value)
    {
        ensure_size(m_values, index + 1);
        m_values[index].first = value;
    }

    std::uint8_t storage::check_at(const std::size_t index) const
    {
        ensure_size(m_values, index + 1);
        return m_values[index].second;
    }

    void storage::set_check_at(const std::size_t index, const std::uint8_t value)
    {
        ensure_size(m_values, index + 1);
        m_values[index].second = value;
    }

    std::size_t storage::size() const
    {
        return m_values.size();
    }


}
