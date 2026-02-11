/*! \file
    \brief An input.

    Copyright (C) 2019-2026 kaoru  https://www.tetengo.org/
*/

#include <cassert>
#include <cstddef>
#include <memory>
#include <typeinfo>
#include <utility>

#include <tetengo/lattice/input.hpp>


namespace tetengo::lattice
{
    input::~input() = default;

    bool operator==(const input& one, const input& another)
    {
        if (typeid(one) != typeid(another))
        {
            return false;
        }
        assert(one.equal_to_impl(another) == another.equal_to_impl(one));
        return one.equal_to_impl(another);
    }

    std::size_t input::hash_value() const
    {
        return hash_value_impl();
    }

    std::size_t input::length() const
    {
        return length_impl();
    }

    std::unique_ptr<input> input::clone() const
    {
        return clone_impl();
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
