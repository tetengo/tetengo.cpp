/*! \file
    \brief An input.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <cstddef>
#include <memory>
#include <utility>

#include <tetengo/lattice/input.hpp>


namespace tetengo::lattice
{
    input::~input() = default;

    std::size_t input::length() const
    {
        return length_impl();
    }

    void input::append(std::unique_ptr<input>&& p_another)
    {
        return append_impl(std::move(p_another));
    }


}
