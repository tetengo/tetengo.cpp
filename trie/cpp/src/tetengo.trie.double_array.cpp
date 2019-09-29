/*! \file
    \brief A double array.

    Copyright (C) 2019 kaoru
*/

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <tetengo/trie/double_array.hpp>

#include "tetengo.trie.double_array_builder.hpp"


namespace tetengo::trie
{
    namespace
    {
        std::int32_t base_at(const std::vector<std::uint32_t>& base_check_array, const std::size_t index)
        {
            return static_cast<std::int32_t>(base_check_array[index]) >> 8;
        }

        std::uint8_t check_at(const std::vector<std::uint32_t>& base_check_array, const std::size_t index)
        {
            return base_check_array[index] & 0xFF;
        }

    }

    double_array::double_array() :
    m_base_check_array{ double_array_builder::build(std::vector<const std::pair<std::string, std::int32_t>*>{}) }
    {}

    double_array::double_array(std::vector<const std::pair<std::string, std::int32_t>*> element_pointers) :
    m_base_check_array{ double_array_builder::build(std::move(element_pointers)) }
    {}

    double_array::double_array(const std::vector<std::pair<std::string, std::int32_t>>& elements) :
    m_base_check_array{ double_array_builder::build(elements) }
    {}

    const std::vector<std::uint32_t>& double_array::base_check_array() const
    {
        return m_base_check_array;
    }

    std::optional<std::int32_t> double_array::find(const std::string& key) const
    {
        std::size_t index = 0;
        for (const auto c: key + '\0')
        {
            const auto next_index = static_cast<std::size_t>(base_at(m_base_check_array, index)) + c;
            if (next_index >= m_base_check_array.size() || check_at(m_base_check_array, next_index) != c)
            {
                return std::nullopt;
            }
            index = next_index;
        }

        return std::make_optional(base_at(m_base_check_array, index));
    }

    double_array_enumerator double_array::enumerator() const
    {
        return double_array_enumerator{};
    }


}
