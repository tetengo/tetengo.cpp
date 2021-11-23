/*! \file
    \brief An input.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <cstddef>

#include <tetengo/lattice/input.hpp>


namespace tetengo::lattice
{
    input_base::~input_base() = default;

    std::size_t input_base::length() const
    {
        return length_impl();
    }


}
