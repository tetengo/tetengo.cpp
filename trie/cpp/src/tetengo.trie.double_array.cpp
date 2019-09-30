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
#include <tetengo/trie/enumerator.hpp>
#include <tetengo/trie/storage.hpp>

#include "tetengo.trie.double_array_builder.hpp"


namespace tetengo::trie
{
    double_array::double_array() :
    m_storage{ double_array_builder::build(std::vector<const std::pair<std::string, std::int32_t>*>{}) }
    {}

    double_array::double_array(std::vector<const std::pair<std::string, std::int32_t>*> element_pointers) :
    m_storage{ double_array_builder::build(std::move(element_pointers)) }
    {}

    double_array::double_array(const std::vector<std::pair<std::string, std::int32_t>>& elements) :
    m_storage{ double_array_builder::build(elements) }
    {}

    const std::vector<std::uint32_t>& double_array::base_check_array() const
    {
        return m_storage.values();
    }

    std::optional<std::int32_t> double_array::find(const std::string& key) const
    {
        std::size_t index = 0;
        for (const auto c: key + double_array::key_terminator())
        {
            const auto next_index = static_cast<std::size_t>(m_storage.base_at(index)) + c;
            if (next_index >= m_storage.size() || m_storage.check_at(next_index) != c)
            {
                return std::nullopt;
            }
            index = next_index;
        }

        return std::make_optional(m_storage.base_at(index));
    }

    enumerator double_array::get_enumerator() const
    {
        return enumerator{ m_storage };
    }


}
