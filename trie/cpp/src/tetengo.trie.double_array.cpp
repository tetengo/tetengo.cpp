/*! \file
    \brief A double array.

    Copyright (C) 2019 kaoru
*/

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include <tetengo/trie/double_array.hpp>

#include "tetengo.trie.double_array_builder.hpp"


namespace tetengo::trie
{
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
}
