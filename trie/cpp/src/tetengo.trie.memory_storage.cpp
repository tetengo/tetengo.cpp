/*! \file
    \brief A memory storage.

    Copyright (C) 2019 kaoru
*/

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <istream>
#include <iterator>
#include <limits>
#include <memory>
#include <optional>
#include <vector>

#include <boost/core/noncopyable.hpp>

#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/memory_storage.hpp>
#include <tetengo/trie/storage.hpp>


namespace tetengo::trie
{
    class memory_storage::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        impl() :
        m_base_check_array{ 0x00000000U | double_array::vacant_check_value() },
            m_mapped_storage_mappings{},
            m_next_mapped_storage_index{ 0 } {};

        explicit impl(std::istream& input_stream) :
        m_base_check_array{},
            m_mapped_storage_mappings{},
            m_next_mapped_storage_index{ 0 }
        {
            deserialize(input_stream, m_base_check_array, m_mapped_storage_mappings, m_next_mapped_storage_index);
        };


        // functions

        std::int32_t base_at_impl(const std::size_t base_check_index) const
        {
            ensure_size(base_check_index + 1);
            return static_cast<std::int32_t>(m_base_check_array[base_check_index]) >> 8;
        }

        void set_base_at_impl(const std::size_t base_check_index, const std::int32_t value)
        {
            ensure_size(base_check_index + 1);
            m_base_check_array[base_check_index] &= 0x000000FF;
            m_base_check_array[base_check_index] |= static_cast<std::uint32_t>(value << 8);
        }

        std::uint8_t check_at_impl(const std::size_t base_check_index) const
        {
            ensure_size(base_check_index + 1);
            return m_base_check_array[base_check_index] & 0xFF;
        }

        void set_check_at_impl(const std::size_t base_check_index, const std::uint8_t value)
        {
            ensure_size(base_check_index + 1);
            m_base_check_array[base_check_index] &= 0xFFFFFF00;
            m_base_check_array[base_check_index] |= value;
        }

        double filling_rate_impl() const
        {
            const auto empty_count =
                std::count(std::begin(m_base_check_array), std::end(m_base_check_array), 0x000000FFU);
            return 1.0 - static_cast<double>(empty_count) / m_base_check_array.size();
        }

        const std::vector<std::uint32_t>& base_check_array_impl() const
        {
            return m_base_check_array;
        }

        std::optional<std::size_t> mapped_storage_index_impl(const std::size_t mapped_index) const
        {
            if (mapped_index >= m_mapped_storage_mappings.size() ||
                m_mapped_storage_mappings[mapped_index] == std::numeric_limits<std::size_t>::max())
            {
                return std::nullopt;
            }
            return m_mapped_storage_mappings[mapped_index];
        }

        void add_mapped_storage_index_impl(const std::size_t mapped_index)
        {
            if (mapped_index >= m_mapped_storage_mappings.size())
            {
                m_mapped_storage_mappings.resize(mapped_index + 1, std::numeric_limits<std::size_t>::max());
            }
            m_mapped_storage_mappings[mapped_index] = m_next_mapped_storage_index;
            ++m_next_mapped_storage_index;
        }

        void serialize_impl(std::ostream& output_stream) const
        {
            {
                write_uint32(output_stream, static_cast<std::uint32_t>(m_base_check_array.size()));
                for (const auto v: m_base_check_array)
                {
                    write_uint32(output_stream, v);
                }
            }
            {
                std::vector<std::uint32_t> values{};
                for (auto i = static_cast<std::uint32_t>(0); i < m_mapped_storage_mappings.size(); ++i)
                {
                    if (m_mapped_storage_mappings[i] == std::numeric_limits<std::size_t>::max())
                    {
                        continue;
                    }

                    if (m_mapped_storage_mappings[i] >= values.size())
                    {
                        values.resize(m_mapped_storage_mappings[i] + 1, std::numeric_limits<std::uint32_t>::max());
                    }
                    values[m_mapped_storage_mappings[i]] = i;
                }
                assert(
                    std::find(values.begin(), values.end(), std::numeric_limits<std::uint32_t>::max()) == values.end());

                write_uint32(output_stream, static_cast<std::uint32_t>(values.size()));
                for (const auto v: values)
                {
                    write_uint32(output_stream, v);
                }
            }
        }

        std::unique_ptr<storage> clone_impl() const
        {
            auto p_clone = std::make_unique<memory_storage>();
            p_clone->m_p_impl->m_base_check_array = m_base_check_array;
            return p_clone;
        }


    private:
        // static functions

        static std::uint32_t read_uint32(std::istream& input_stream)
        {
            std::array<char, 4> buffer{};
            input_stream.read(buffer.data(), buffer.size());
            if (input_stream.gcount() < static_cast<std::streamsize>(buffer.size()))
            {
                throw std::ios_base::failure("Can't read uint32.");
            }

            return (static_cast<std::uint8_t>(buffer[0]) << 24) | (static_cast<std::uint8_t>(buffer[1]) << 16) |
                   (static_cast<std::uint8_t>(buffer[2]) << 8) | static_cast<std::uint8_t>(buffer[3]);
        }

        static void write_uint32(std::ostream& output_stream, const std::uint32_t value)
        {
            const std::array<char, 4> buffer{ static_cast<char>((value & 0xFF000000) >> 24),
                                              static_cast<char>((value & 0x00FF0000) >> 16),
                                              static_cast<char>((value & 0x0000FF00) >> 8),
                                              static_cast<char>(value & 0x00FF00FF) };
            output_stream.write(buffer.data(), buffer.size());
        }

        static void deserialize(
            std::istream&               input_stream,
            std::vector<std::uint32_t>& base_check_array,
            std::vector<std::size_t>&   mapped_storage_mappings,
            std::size_t&                next_mapped_storage_index)
        {
            {
                const auto size = read_uint32(input_stream);
                base_check_array.reserve(size);
                for (auto i = static_cast<std::uint32_t>(0); i < size; ++i)
                {
                    base_check_array.push_back(read_uint32(input_stream));
                }
            }
            {
                const auto size = read_uint32(input_stream);
                for (auto i = static_cast<std::uint32_t>(0); i < size; ++i)
                {
                    const auto value = read_uint32(input_stream);
                    if (value >= mapped_storage_mappings.size())
                    {
                        mapped_storage_mappings.resize(value + 1, std::numeric_limits<std::size_t>::max());
                    }
                    mapped_storage_mappings[value] = i;
                }
                next_mapped_storage_index = size;
            }
        }


        // variables

        mutable std::vector<std::uint32_t> m_base_check_array;

        std::vector<std::size_t> m_mapped_storage_mappings;

        std::size_t m_next_mapped_storage_index;


        // functions

        void ensure_size(const std::size_t size) const
        {
            if (size > m_base_check_array.size())
            {
                m_base_check_array.resize(size, 0x00000000U | double_array::vacant_check_value());
            }
        }
    };


    memory_storage::memory_storage() : m_p_impl{ std::make_unique<impl>() } {}

    memory_storage::memory_storage(std::istream& input_stream) : m_p_impl{ std::make_unique<impl>(input_stream) } {}

    memory_storage::~memory_storage() = default;

    std::int32_t memory_storage::base_at_impl(const std::size_t base_check_index) const
    {
        return m_p_impl->base_at_impl(base_check_index);
    }

    void memory_storage::set_base_at_impl(const std::size_t base_check_index, const std::int32_t value)
    {
        m_p_impl->set_base_at_impl(base_check_index, value);
    }

    std::uint8_t memory_storage::check_at_impl(const std::size_t base_check_index) const
    {
        return m_p_impl->check_at_impl(base_check_index);
    }

    void memory_storage::set_check_at_impl(const std::size_t base_check_index, const std::uint8_t value)
    {
        m_p_impl->set_check_at_impl(base_check_index, value);
    }

    double memory_storage::filling_rate_impl() const
    {
        return m_p_impl->filling_rate_impl();
    }

    const std::vector<std::uint32_t>& memory_storage::base_check_array_impl() const
    {
        return m_p_impl->base_check_array_impl();
    }

    std::optional<std::size_t> memory_storage::mapped_storage_index_impl(const std::size_t mapped_index) const
    {
        return m_p_impl->mapped_storage_index_impl(mapped_index);
    }

    void memory_storage::add_mapped_storage_index_impl(const std::size_t mapped_index)
    {
        return m_p_impl->add_mapped_storage_index_impl(mapped_index);
    }

    void memory_storage::serialize_impl(std::ostream& output_stream) const
    {
        m_p_impl->serialize_impl(output_stream);
    }

    std::unique_ptr<storage> memory_storage::clone_impl() const
    {
        return m_p_impl->clone_impl();
    }


}
