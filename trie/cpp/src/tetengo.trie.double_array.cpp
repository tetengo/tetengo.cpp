/*! \file
    \brief A double array.

    Copyright (C) 2019 kaoru
*/

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

#include <tetengo/trie/double_array.hpp>


namespace tetengo::trie
{
    namespace
    {
        using element_vector_type = std::vector<const std::pair<std::string, std::int32_t>*>;

        using element_iterator_type = element_vector_type::const_iterator;

        char char_code_at(const std::string& string, const std::size_t index)
        {
            return index < string.length() ? string[index] : '\0';
        }

        std::vector<element_iterator_type>
        children_firsts(const element_iterator_type first, const element_iterator_type last, const std::size_t offset)
        {
            std::vector<element_iterator_type> firsts{ first };
            for (auto child_first = first; child_first != last;)
            {
                auto child_last = std::find_if(child_first, last, [child_first, offset](const auto& e) {
                    return char_code_at(e->first, offset) != char_code_at((*child_first)->first, offset);
                });

                firsts.push_back(child_last);

                child_first = child_last;
            }
            assert(!firsts.empty());
            return firsts;
        }

        //std::int32_t base_at(const std::vector<std::uint32_t>& base_check_array, const std::size_t index)
        //{
        //    const auto value = index < base_check_array.size() ? static_cast<std::int32_t>(base_check_array[index]) : 0;
        //    return value >> 8;
        //}

        void
        set_base_at(std::vector<std::uint32_t>& base_check_array, const std::size_t index, const std::int32_t value)
        {
            if (index >= base_check_array.size())
            {
                base_check_array.resize(index + 1, 0x000000FF);
            }
            const uint32_t array_value = value << 8;
            base_check_array[index] &= 0x000000FF;
            base_check_array[index] |= array_value;
        }

        std::uint8_t check_at(const std::vector<std::uint32_t>& base_check_array, const std::size_t index)
        {
            const auto value = index < base_check_array.size() ? base_check_array[index] : 0xFF;
            return value & 0xFF;
        }

        void
        set_check_at(std::vector<std::uint32_t>& base_check_array, const std::size_t index, const std::uint8_t value)
        {
            if (index >= base_check_array.size())
            {
                base_check_array.resize(index + 1, 0x000000FF);
            }
            const uint32_t array_value = value;
            base_check_array[index] &= 0xFFFFFF00;
            base_check_array[index] |= array_value;
        }

        std::int32_t calc_base(
            const std::vector<element_iterator_type>& firsts,
            const std::size_t                         offset,
            const std::vector<std::uint32_t>&         base_check_array)
        {
            for (auto base = -char_code_at((*firsts[0])->first, offset) + 1;; ++base)
            {
                bool vacant = 1;
                for (auto i = firsts.begin(); std::next(i) != firsts.end(); ++i)
                {
                    const auto next_index = base + char_code_at((**i)->first, offset);
                    if (check_at(base_check_array, next_index) != 0xFF)
                    {
                        vacant = false;
                        break;
                    }
                }

                if (vacant)
                {
                    return base;
                }
            }
        }

        void build_base_check_array_iter(
            const element_iterator_type first,
            const element_iterator_type last,
            const std::size_t           offset,
            std::vector<std::uint32_t>& base_check_array,
            const std::size_t           base_check_array_index)
        {
            const auto children_firsts_ = children_firsts(first, last, offset);

            const auto base = calc_base(children_firsts_, offset, base_check_array);
            set_base_at(base_check_array, base_check_array_index, base);

            for (auto i = children_firsts_.begin(); std::next(i) != children_firsts_.end(); ++i)
            {
                const auto char_code = char_code_at((**i)->first, offset);
                const auto next_base_check_array_index = base + char_code;
                set_check_at(base_check_array, next_base_check_array_index, char_code);
            }
            for (auto i = children_firsts_.begin(); std::next(i) != children_firsts_.end(); ++i)
            {
                const auto char_code = char_code_at((**i)->first, offset);
                const auto next_base_check_array_index = base + char_code;
                if (char_code == '\0')
                {
                    set_base_at(base_check_array, next_base_check_array_index, (**i)->second);
                    continue;
                }
                build_base_check_array_iter(
                    *i, *std::next(i), offset + 1, base_check_array, next_base_check_array_index);
            }
        }

        std::vector<std::uint32_t>
        build_base_check_array(std::initializer_list<std::pair<std::string, std::int32_t>> elements)
        {
            std::vector<std::uint32_t> base_check_array{ 0x000000FF };

            element_vector_type element_vector{};
            element_vector.reserve(elements.size());
            std::transform(
                elements.begin(), elements.end(), std::back_inserter(element_vector), [](const auto& e) { return &e; });
            std::stable_sort(element_vector.begin(), element_vector.end(), [](const auto& e1, const auto& e2) {
                return e1->first < e2->first;
            });

            if (!element_vector.empty())
            {
                build_base_check_array_iter(element_vector.begin(), element_vector.end(), 0, base_check_array, 0);
            }

            return base_check_array;
        }
    }


    double_array::double_array() : m_base_check_array{ build_base_check_array({}) } {}

    double_array::double_array(std::initializer_list<std::pair<std::string, std::int32_t>> elements) :
    m_base_check_array{ build_base_check_array(std::move(elements)) }
    {}

    const std::vector<std::uint32_t>& double_array::base_check_array() const
    {
        return m_base_check_array;
    }
}
