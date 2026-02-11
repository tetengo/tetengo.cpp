/*! \file
    \brief A double array iterator.

    Copyright (C) 2019-2026 kaoru  https://www.tetengo.org/
*/

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
#include <optional>
#include <stack>
#include <stdexcept>
#include <string>
#include <type_traits> // IWYU pragma: keep
#include <utility>
#include <vector>

#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/double_array_iterator.hpp>
#include <tetengo/trie/storage.hpp>


namespace tetengo::trie
{
    double_array_iterator::double_array_iterator() : m_p_storage{ nullptr }, m_base_check_index_key_stack{}, m_current{}
    {}

    double_array_iterator::double_array_iterator(const storage& storage_, const std::size_t root_base_check_index) :
    m_p_storage{ &storage_ },
    m_base_check_index_key_stack{ std::vector<std::pair<std::size_t, std::string>>{
        std::make_pair(root_base_check_index, std::string{}) } },
    m_current{}
    {
        operator++();
    }

    const std::int32_t& double_array_iterator::operator*() const
    {
        if (!m_current)
        {
            throw std::logic_error{ "Iterator not dereferenceable." };
        }
        return *m_current;
    }

    std::int32_t& double_array_iterator::operator*()
    {
        if (!m_current)
        {
            throw std::logic_error{ "Iterator not dereferenceable." };
        }
        return *m_current;
    }

    const std::int32_t* double_array_iterator::operator->() const
    {
        return &operator*();
    }

    std::int32_t* double_array_iterator::operator->()
    {
        return &operator*();
    }

    bool operator==(const double_array_iterator& one, const double_array_iterator& another)
    {
        if ((!one.m_p_storage || !another.m_p_storage) && std::empty(one.m_base_check_index_key_stack) &&
            std::empty(another.m_base_check_index_key_stack) && !one.m_current && !another.m_current)
        {
            return true;
        }
        return one.m_p_storage == another.m_p_storage &&
               one.m_base_check_index_key_stack == another.m_base_check_index_key_stack;
    }

    double_array_iterator& double_array_iterator::operator++()
    {
        m_current = next();
        return *this;
    }

    std::optional<std::int32_t> double_array_iterator::next()
    {
        if (std::empty(m_base_check_index_key_stack))
        {
            return std::nullopt;
        }

        const auto base_check_index = m_base_check_index_key_stack.top().first;
        const auto key = m_base_check_index_key_stack.top().second;
        m_base_check_index_key_stack.pop();

        const auto base = m_p_storage->base_at(base_check_index);
        const auto check = m_p_storage->check_at(base_check_index);

        if (check == double_array::key_terminator())
        {
            return std::make_optional(base);
        }

        for (auto char_code = static_cast<std::int32_t>(0xFE); char_code >= 0; --char_code)
        {
            const auto char_code_as_uint8 = static_cast<std::uint8_t>(static_cast<char>(char_code));
            const auto next_index = base + char_code_as_uint8;
            if (next_index < 0)
            {
                continue;
            }
            if (m_p_storage->check_at(next_index) == char_code_as_uint8)
            {
                const auto next_key_tail = char_code_as_uint8 != double_array::key_terminator() ?
                                               std::string{ static_cast<char>(char_code_as_uint8) } :
                                               std::string{};
                m_base_check_index_key_stack.push(std::make_pair(next_index, key + next_key_tail));
            }
        }

        return next();
    }

    double_array_iterator double_array_iterator::operator++(int)
    {
        double_array_iterator original{ *this };
        ++(*this);
        return original;
    }


}
