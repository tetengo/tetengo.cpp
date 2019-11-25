/*! \file
    \brief A storage.

    Copyright (C) 2019 kaoru
*/

#include <cstdint>
#include <istream>
#include <memory>
#include <vector>

#include <tetengo/trie/storage.hpp>


namespace tetengo::trie
{
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

    void storage::set_check_at(const std::size_t base_check_index, const std::uint8_t value)
    {
        set_check_at_impl(base_check_index, value);
    }

    double storage::filling_rate() const
    {
        return filling_rate_impl();
    }

    const std::vector<std::uint32_t>& storage::base_check_array() const
    {
        return base_check_array_impl();
    }

    void storage::serialize(std::ostream& output_stream) const
    {
        serialize_impl(output_stream);
    }

    std::unique_ptr<storage> storage::clone() const
    {
        return clone_impl();
    }


}
