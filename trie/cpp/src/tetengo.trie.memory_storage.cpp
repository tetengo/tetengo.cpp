/*! \file
    \brief A memory storage.

    Copyright (C) 2019 kaoru
*/

#include <algorithm>
#include <any>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <istream>
#include <iterator>
#include <limits>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <boost/core/noncopyable.hpp>

#include <tetengo/trie/default_serializer.hpp>
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
        m_mapped_index_mappings{},
        m_mapped_array{} {};

        explicit impl(std::istream& input_stream) : m_base_check_array{}, m_mapped_index_mappings{}, m_mapped_array{}
        {
            deserialize(input_stream, m_base_check_array, m_mapped_index_mappings, m_mapped_array);
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

        const std::any* mapped_at_impl(const std::size_t mapped_index) const
        {
            if (mapped_index >= m_mapped_index_mappings.size() ||
                m_mapped_index_mappings[mapped_index] == std::numeric_limits<std::size_t>::max())
            {
                return nullptr;
            }
            assert(m_mapped_index_mappings[mapped_index] < m_mapped_array.size());
            return &m_mapped_array[m_mapped_index_mappings[mapped_index]];
        }

        void add_mapped_at_impl(const std::size_t mapped_index, std::any mapped)
        {
            if (mapped_index >= m_mapped_index_mappings.size())
            {
                m_mapped_index_mappings.resize(mapped_index + 1, std::numeric_limits<std::size_t>::max());
            }
            m_mapped_index_mappings[mapped_index] = m_mapped_array.size();
            m_mapped_array.push_back(std::move(mapped));
        }

        void serialize_impl(
            std::ostream& output_stream,
            const std::function<std::string(const std::any&)>& /*mapped_serializer*/) const
        {
            serialize_base_check_array(output_stream, m_base_check_array);
            serialize_mapped_index_mappings(output_stream, m_mapped_index_mappings);
            serialize_mapped_array(output_stream, m_mapped_array);
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

        static void
        serialize_base_check_array(std::ostream& output_stream, const std::vector<std::uint32_t>& base_check_array)
        {
            static const default_serializer<std::uint32_t> uint32_serializer{};
            {
                assert(base_check_array.size() < std::numeric_limits<std::uint32_t>::max());
                const auto serialized = uint32_serializer(static_cast<std::uint32_t>(base_check_array.size()));
                output_stream.write(serialized.data(), serialized.length());
            }
            for (const auto v: base_check_array)
            {
                const auto serialized = uint32_serializer(v);
                output_stream.write(serialized.data(), serialized.length());
            }
        }

        static void serialize_mapped_index_mappings(
            std::ostream&                   output_stream,
            const std::vector<std::size_t>& mapped_index_mappings)
        {
            std::vector<std::uint32_t> values{};
            for (auto i = static_cast<std::uint32_t>(0); i < mapped_index_mappings.size(); ++i)
            {
                if (mapped_index_mappings[i] == std::numeric_limits<std::size_t>::max())
                {
                    continue;
                }

                if (mapped_index_mappings[i] >= values.size())
                {
                    values.resize(mapped_index_mappings[i] + 1, std::numeric_limits<std::uint32_t>::max());
                }
                values[mapped_index_mappings[i]] = i;
            }
            assert(std::find(values.begin(), values.end(), std::numeric_limits<std::uint32_t>::max()) == values.end());

            static const default_serializer<std::uint32_t> uint32_serializer{};
            {
                assert(values.size() < std::numeric_limits<std::uint32_t>::max());
                const auto serialized = uint32_serializer(static_cast<std::uint32_t>(values.size()));
                output_stream.write(serialized.data(), serialized.length());
            }
            for (const auto v: values)
            {
                const auto serialized = uint32_serializer(v);
                output_stream.write(serialized.data(), serialized.length());
            }
        }

        static void
        serialize_mapped_array(std::ostream& /*output_stream*/, const std::vector<std::any>& /*mapped_array*/)
        {}

        static void deserialize(
            std::istream&               input_stream,
            std::vector<std::uint32_t>& base_check_array,
            std::vector<std::size_t>&   mapped_index_mappings,
            std::vector<std::any>& /*mapped_array*/)
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
                    if (value >= mapped_index_mappings.size())
                    {
                        mapped_index_mappings.resize(value + 1, std::numeric_limits<std::size_t>::max());
                    }
                    mapped_index_mappings[value] = i;
                }
            }
        }


        // variables

        mutable std::vector<std::uint32_t> m_base_check_array;

        std::vector<std::size_t> m_mapped_index_mappings;

        std::vector<std::any> m_mapped_array;


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

    const std::any* memory_storage::mapped_at_impl(const std::size_t mapped_index) const
    {
        return m_p_impl->mapped_at_impl(mapped_index);
    }

    void memory_storage::add_mapped_at_impl(const std::size_t mapped_index, std::any mapped)
    {
        return m_p_impl->add_mapped_at_impl(mapped_index, std::move(mapped));
    }

    void memory_storage::serialize_impl(
        std::ostream&                                      output_stream,
        const std::function<std::string(const std::any&)>& mapped_serializer) const
    {
        m_p_impl->serialize_impl(output_stream, mapped_serializer);
    }

    std::unique_ptr<storage> memory_storage::clone_impl() const
    {
        return m_p_impl->clone_impl();
    }


}
