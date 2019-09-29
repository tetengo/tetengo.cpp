/*! \file
    \brief A double array.

    Copyright (C) 2019 kaoru
*/

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <optional>
#include <stack>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/storage.hpp>

#include "tetengo.trie.double_array_builder.hpp"


namespace tetengo::trie
{
    double_array_enumerator::double_array_enumerator(const storage& storage_) :
    m_storage{ storage_ },
        m_index_key_stack{ std::vector<std::pair<std::size_t, std::string>>{ std::make_pair(0, std::string{}) } }
    {}

    std::optional<std::pair<std::string, std::int32_t>> double_array_enumerator::next() const
    {
        if (m_index_key_stack.empty())
        {
            return std::nullopt;
        }

        const auto index = m_index_key_stack.top().first;
        const auto key = m_index_key_stack.top().second;
        m_index_key_stack.pop();

        const auto base = m_storage.base_at(index);

        std::vector<std::pair<std::size_t, std::string>> children_indexes_and_keys{};
        for (auto next_index = std::max(0, base);
             next_index <
             std::min(base + std::numeric_limits<std::uint8_t>::max(), static_cast<std::int32_t>(m_storage.size()));
             ++next_index)
        {
            const auto char_code = m_storage.check_at(next_index);
            const auto next_index_to_check = base + char_code;
            if (next_index_to_check == next_index)
            {
                const auto next_key_tail = char_code != double_array::key_terminator() ?
                                               std::string{ static_cast<char>(char_code) } :
                                               std::string{};
                children_indexes_and_keys.push_back(std::make_pair(next_index, key + next_key_tail));
            }
        }

        if (!children_indexes_and_keys.empty())
        {
            std::for_each(children_indexes_and_keys.rbegin(), children_indexes_and_keys.rend(), [this](auto& e) {
                m_index_key_stack.push(std::move(e));
            });
            return next();
        }
        else
        {
            if (m_storage.check_at(index) == double_array::vacant_check_value())
            {
                return std::nullopt;
            }
            else
            {
                assert(m_storage.check_at(index) == double_array::key_terminator());
                return std::make_optional(std::make_pair(key, base));
            }
        }
    }

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

    double_array_enumerator double_array::enumerator() const
    {
        return double_array_enumerator{ m_storage };
    }


}
