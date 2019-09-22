/*! \file
    \brief A double array.

    Copyright (C) 2019 kaoru
*/

#include <cstdint>
#include <initializer_list>
#include <string>
#include <utility>
#include <vector>

#include <tetengo/trie/double_array.hpp>


namespace tetengo::trie
{
    double_array::double_array() : m_base_check_array{} {}

    double_array::double_array(std::initializer_list<std::pair<std::string, std::int32_t>> /*elements*/) :
    m_base_check_array{}
    {}

    const std::vector<std::uint32_t> double_array::base_check_array() const
    {
        return {};
    }

}
