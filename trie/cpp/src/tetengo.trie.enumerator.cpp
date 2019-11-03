/*! \file
    \brief An enumerator.

    Copyright (C) 2019 kaoru
*/

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <stack>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/enumerator.hpp>
#include <tetengo/trie/storage.hpp>


namespace tetengo::trie
{
    enumerator::enumerator(const storage& storage_, std::size_t root_index) :
    m_storage{ storage_ },
        m_index_key_stack{ std::vector<std::pair<std::size_t, std::string>>{
            std::make_pair(root_index, std::string{}) } }
    {}

    std::optional<std::pair<std::string, std::int32_t>> enumerator::next() const
    {
        if (m_index_key_stack.empty())
        {
            return std::nullopt;
        }

        const auto index = m_index_key_stack.top().first;
        const auto key = m_index_key_stack.top().second;
        m_index_key_stack.pop();

        const auto base = m_storage.base_at(index);
        const auto check = m_storage.check_at(index);

        if (check == double_array::key_terminator())
        {
            return std::make_optional(std::make_pair(key, base));
        }

        for (auto char_code = static_cast<std::int32_t>(0xFE); char_code >= 0; --char_code)
        {
            const auto char_code_as_uint8 = static_cast<std::uint8_t>(static_cast<char>(char_code));
            const auto next_index = base + char_code_as_uint8;
            if (next_index < 0)
            {
                continue;
            }
            if (m_storage.check_at(next_index) == char_code_as_uint8)
            {
                const auto next_key_tail = char_code_as_uint8 != double_array::key_terminator() ?
                                               std::string{ static_cast<char>(char_code_as_uint8) } :
                                               std::string{};
                m_index_key_stack.push(std::make_pair(next_index, key + next_key_tail));
            }
        }

        return next();
    }


}
