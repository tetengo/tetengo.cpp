/*! \file
    \brief A storage.

    Copyright (C) 2019 kaoru
*/

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/storage.hpp>


namespace tetengo::trie
{
    namespace
    {
        void ensure_size(std::vector<std::uint32_t>& values, const std::size_t size)
        {
            if (size > values.size())
            {
                values.resize(size, 0x00000000U | double_array::vacant_check_value());
            }
        }


    }


    storage::storage() : m_values{} {}

    std::int32_t storage::base_at(const std::size_t index) const
    {
        ensure_size(m_values, index + 1);
        return static_cast<std::int32_t>(m_values[index]) >> 8;
    }

    void storage::set_base_at(const std::size_t index, const std::int32_t value)
    {
        ensure_size(m_values, index + 1);
        m_values[index] &= 0x000000FF;
        m_values[index] |= static_cast<std::uint32_t>(value << 8);
    }

    std::uint8_t storage::check_at(const std::size_t index) const
    {
        ensure_size(m_values, index + 1);
        return m_values[index] & 0xFF;
    }

    void storage::set_check_at(const std::size_t index, const std::uint8_t value)
    {
        ensure_size(m_values, index + 1);
        m_values[index] &= 0xFFFFFF00;
        m_values[index] |= value;
    }

    std::size_t storage::size() const
    {
        return m_values.size();
    }

    const std::vector<std::uint32_t>& storage::values() const
    {
        return m_values;
    }


}
