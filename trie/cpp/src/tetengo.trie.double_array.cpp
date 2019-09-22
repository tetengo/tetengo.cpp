/*! \file
    \brief A double array.

    Copyright (C) 2019 kaoru
*/

#include <algorithm>
#include <cstdint>
#include <initializer_list>
#include <string>
#include <utility>
#include <vector>

#include <tetengo/trie/double_array.hpp>


namespace tetengo::trie
{
    namespace
    {
        std::vector<std::uint32_t>
        build_base_check_array(std::initializer_list<std::pair<std::string, std::int32_t>> elements)
        {
            std::vector<std::uint32_t> base_check_array{ 0x000000FF };

            std::vector<std::pair<std::string, std::int32_t>> element_array{ std::move(elements) };
            std::stable_sort(element_array.begin(), element_array.end(), [](const auto& e1, const auto& e2) {
                return e1.first < e2.first;
            });

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
