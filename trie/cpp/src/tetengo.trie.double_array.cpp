/*! \file
    \brief A double array.

    Copyright (C) 2019 kaoru
*/

#include <cassert>
#include <cstdint>

#include <tetengo/trie/double_array.hpp>


namespace tetengo::trie
{
    std::uint32_t double_array::make_base_check(const std::int32_t base, const std::int8_t check)
    {
        assert(-8388608 <= base && base < 8388608);

        const std::uint32_t ubase = static_cast<std::uint32_t>(base);
        const std::uint8_t  ucheck = static_cast<std::uint8_t>(check);

        return (ubase << 8) | ucheck;
    }

    std::int32_t double_array::base_of(const std::uint32_t base_check)
    {
        return static_cast<std::int32_t>(base_check) >> 8;
    }

    std::int8_t double_array::check_of(const std::uint32_t base_check)
    {
        return static_cast<std::int8_t>(base_check & 0xFF);
    }

    double_array::double_array() {}


}
