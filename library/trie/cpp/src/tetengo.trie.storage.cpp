/*! \file
    \brief A storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <cstdint>
#include <istream>
#include <memory>
#include <utility>

#include <tetengo/trie/storage.hpp>


namespace tetengo::trie
{
    class value_serializer;


    storage::storage() = default;

    storage::~storage() = default;

    std::size_t storage::base_check_size() const
    {
        return base_check_size_impl();
    }

    std::int32_t storage::base_at(const std::size_t base_check_index) const
    {
        return base_at_impl(base_check_index);
    }

    void storage::set_base_at(const std::size_t base_check_index, const std::int32_t value)
    {
        set_base_at_impl(base_check_index, value);
    }

    std::uint8_t storage::check_at(const std::size_t base_check_index) const
    {
        return check_at_impl(base_check_index);
    }

    void storage::set_check_at(const std::size_t base_check_index, const std::uint8_t base)
    {
        set_check_at_impl(base_check_index, base);
    }

    std::size_t storage::size() const
    {
        return size_impl();
    }

    double storage::filling_rate() const
    {
        return filling_rate_impl();
    }

    const std::any* storage::value_at(const std::size_t value_index) const
    {
        return value_at_impl(value_index);
    }

    void storage::add_value_at(const std::size_t value_index, std::any value)
    {
        add_value_at_impl(value_index, std::move(value));
    }

    void storage::serialize(std::ostream& output_stream, const value_serializer& value_serializer_) const
    {
        serialize_impl(output_stream, value_serializer_);
    }

    std::unique_ptr<storage> storage::clone() const
    {
        return clone_impl();
    }


}
