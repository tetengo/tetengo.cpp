/*! \file
    \brief A double array builder.

    Copyright (C) 2019 kaoru
*/

#include <algorithm>
#include <cassert>
#include <iterator>

#include <tetengo/trie/double_array.hpp>

#include "tetengo.trie.double_array_builder.hpp"


namespace tetengo::trie
{
    std::vector<std::uint32_t>
    double_array_builder::build(std::vector<const std::pair<std::string, std::int32_t>*> element_pointers)
    {
        std::vector<std::uint32_t> base_check_array{ 0x00000000U | double_array::vacant_check_value() };

        std::stable_sort(element_pointers.begin(), element_pointers.end(), [](const auto& e1, const auto& e2) {
            return e1->first < e2->first;
        });

        if (!element_pointers.empty())
        {
            build_iter(element_pointers.begin(), element_pointers.end(), 0, base_check_array, 0);
        }

        return base_check_array;
    }

    std::vector<std::uint32_t>
    double_array_builder::build(const std::vector<std::pair<std::string, std::int32_t>>& elements)
    {
        element_vector_type element_pointers{};
        element_pointers.reserve(elements.size());
        std::transform(
            elements.begin(), elements.end(), std::back_inserter(element_pointers), [](const auto& e) { return &e; });

        return build(std::move(element_pointers));
    }

    void double_array_builder::build_iter(
        const element_iterator_type first,
        const element_iterator_type last,
        const std::size_t           key_offset,
        std::vector<std::uint32_t>& base_check_array,
        const std::size_t           base_check_array_index)
    {
        const auto children_firsts_ = children_firsts(first, last, key_offset);

        const auto base = calc_base(children_firsts_, key_offset, base_check_array, base_check_array_index);
        set_base_at(base_check_array, base_check_array_index, base);

        for (auto i = children_firsts_.begin(); i != std::prev(children_firsts_.end()); ++i)
        {
            const auto char_code = char_code_at((**i)->first, key_offset);
            const auto next_base_check_array_index = base + char_code;
            set_check_at(base_check_array, next_base_check_array_index, char_code);
        }
        for (auto i = children_firsts_.begin(); i != std::prev(children_firsts_.end()); ++i)
        {
            const auto char_code = char_code_at((**i)->first, key_offset);
            const auto next_base_check_array_index = base + char_code;
            if (char_code == double_array::key_terminator())
            {
                set_base_at(base_check_array, next_base_check_array_index, (**i)->second);
                continue;
            }
            build_iter(*i, *std::next(i), key_offset + 1, base_check_array, next_base_check_array_index);
        }
    }

    std::int32_t double_array_builder::calc_base(
        const std::vector<element_iterator_type>& firsts,
        const std::size_t                         key_offset,
        const std::vector<std::uint32_t>&         base_check_array,
        const std::size_t                         base_check_array_index)
    {
        for (auto base =
                 -char_code_at((*firsts[0])->first, key_offset) + static_cast<std::int32_t>(base_check_array_index) + 1;
             ;
             ++base)
        {
            const auto first_last = std::prev(firsts.end());
            const auto occupied =
                std::find_if(firsts.begin(), first_last, [key_offset, &base_check_array, base](const auto& first) {
                    const auto next_index = base + char_code_at((*first)->first, key_offset);
                    return check_at(base_check_array, next_index) != double_array::vacant_check_value();
                });
            if (occupied == first_last)
            {
                return base;
            }
        }
    }

    void double_array_builder::set_base_at(
        std::vector<std::uint32_t>& base_check_array,
        const std::size_t           index,
        const std::int32_t          value)
    {
        ensure_base_check_array_size(base_check_array, index + 1);
        const uint32_t array_value = value << 8;
        base_check_array[index] &= (0x00000000U | double_array::vacant_check_value());
        base_check_array[index] |= array_value;
    }

    std::uint8_t
    double_array_builder::check_at(const std::vector<std::uint32_t>& base_check_array, const std::size_t index)
    {
        const auto value =
            index < base_check_array.size() ? base_check_array[index] : double_array::vacant_check_value();
        return value & 0xFF;
    }

    void double_array_builder::set_check_at(
        std::vector<std::uint32_t>& base_check_array,
        const std::size_t           index,
        const std::uint8_t          value)
    {
        ensure_base_check_array_size(base_check_array, index + 1);
        const uint32_t array_value = value;
        base_check_array[index] &= 0xFFFFFF00;
        base_check_array[index] |= array_value;
    }

    void double_array_builder::ensure_base_check_array_size(
        std::vector<std::uint32_t>& base_check_array,
        const std::size_t           size)
    {
        if (size > base_check_array.size())
        {
            base_check_array.resize(size, 0x00000000U | double_array::vacant_check_value());
        }
    }

    std::vector<double_array_builder::element_iterator_type> double_array_builder::children_firsts(
        const element_iterator_type first,
        const element_iterator_type last,
        const std::size_t           key_offset)
    {
        std::vector<element_iterator_type> firsts{ first };
        for (auto child_first = first; child_first != last;)
        {
            auto child_last = std::find_if(child_first, last, [child_first, key_offset](const auto& e) {
                return char_code_at(e->first, key_offset) != char_code_at((*child_first)->first, key_offset);
            });

            firsts.push_back(child_last);

            child_first = child_last;
        }
        assert(!firsts.empty());
        return firsts;
    }

    char double_array_builder::char_code_at(const std::string& string, const std::size_t index)
    {
        return index < string.length() ? string[index] : double_array::key_terminator();
    }
}
