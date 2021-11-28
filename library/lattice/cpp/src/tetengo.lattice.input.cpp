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

    std::unique_ptr<input> input::create_subrange(const std::size_t offset, const std::size_t length) const
    {
        return create_subrange_impl(offset, length);
    }

    void input::append(std::unique_ptr<input>&& p_another)
    {
        return append_impl(std::move(p_another));
    }


}
