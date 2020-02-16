/*! \file
    \brief A memory storage.

    Copyright (C) 2019-2020 kaoru
*/

#include <algorithm>
#include <any>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <istream>
#include <iterator>
#include <limits>
#include <memory>
#include <string>
#include <string_view>
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
            m_value_index_mappings{},
            m_value_array{} {};

        explicit impl(
            std::istream&                                           input_stream,
            const std::function<std::any(const std::string_view&)>& value_deserializer) :
        m_base_check_array{},
            m_value_index_mappings{},
            m_value_array{}
        {
            deserialize(input_stream, value_deserializer, m_base_check_array, m_value_index_mappings, m_value_array);
        };


        // functions

        std::int32_t base_at_impl(const std::size_t base_check_index) const
        {
            ensure_size(base_check_index + 1);
            return static_cast<std::int32_t>(m_base_check_array[base_check_index]) >> 8;
        }

        void set_base_at_impl(const std::size_t base_check_index, const std::int32_t base)
        {
            ensure_size(base_check_index + 1);
            m_base_check_array[base_check_index] &= 0x000000FF;
            m_base_check_array[base_check_index] |= static_cast<std::uint32_t>(base << 8);
        }

        std::uint8_t check_at_impl(const std::size_t base_check_index) const
        {
            ensure_size(base_check_index + 1);
            return m_base_check_array[base_check_index] & 0xFF;
        }

        void set_check_at_impl(const std::size_t base_check_index, const std::uint8_t check)
        {
            ensure_size(base_check_index + 1);
            m_base_check_array[base_check_index] &= 0xFFFFFF00;
            m_base_check_array[base_check_index] |= check;
        }

        std::size_t size_impl() const
        {
            return m_value_array.size();
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

        const std::any* value_at_impl(const std::size_t value_index) const
        {
            if (value_index >= m_value_index_mappings.size() ||
                m_value_index_mappings[value_index] == std::numeric_limits<std::size_t>::max())
            {
                return nullptr;
            }
            assert(m_value_index_mappings[value_index] < m_value_array.size());
            return &m_value_array[m_value_index_mappings[value_index]];
        }

        void add_value_at_impl(const std::size_t value_index, std::any value)
        {
            if (value_index >= m_value_index_mappings.size())
            {
                m_value_index_mappings.resize(value_index + 1, std::numeric_limits<std::size_t>::max());
            }
            m_value_index_mappings[value_index] = m_value_array.size();
            m_value_array.push_back(std::move(value));
        }

        void serialize_impl(
            std::ostream&                                      output_stream,
            const std::function<std::string(const std::any&)>& value_serializer) const
        {
            serialize_base_check_array(output_stream, m_base_check_array);
            serialize_value_index_mappings(output_stream, m_value_index_mappings);
            serialize_value_array(output_stream, value_serializer, m_value_array);
        }

        std::unique_ptr<storage> clone_impl() const
        {
            auto p_clone = std::make_unique<memory_storage>();
            p_clone->m_p_impl->m_base_check_array = m_base_check_array;
            p_clone->m_p_impl->m_value_index_mappings = m_value_index_mappings;
            p_clone->m_p_impl->m_value_array = m_value_array;
            return p_clone;
        }


    private:
        // static functions

        static void
        serialize_base_check_array(std::ostream& output_stream, const std::vector<std::uint32_t>& base_check_array)
        {
            assert(base_check_array.size() < std::numeric_limits<std::uint32_t>::max());
            write_uint32(output_stream, static_cast<std::uint32_t>(base_check_array.size()));
            for (const auto v: base_check_array)
            {
                write_uint32(output_stream, v);
            }
        }

        static void serialize_value_index_mappings(
            std::ostream&                   output_stream,
            const std::vector<std::size_t>& value_index_mappings)
        {
            std::vector<std::uint32_t> values{};
            for (auto i = static_cast<std::uint32_t>(0); i < value_index_mappings.size(); ++i)
            {
                if (value_index_mappings[i] == std::numeric_limits<std::size_t>::max())
                {
                    continue;
                }

                if (value_index_mappings[i] >= values.size())
                {
                    values.resize(value_index_mappings[i] + 1, std::numeric_limits<std::uint32_t>::max());
                }
                values[value_index_mappings[i]] = i;
            }
            assert(
                std::find(std::begin(values), std::end(values), std::numeric_limits<std::uint32_t>::max()) ==
                std::end(values));

            assert(values.size() < std::numeric_limits<std::uint32_t>::max());
            write_uint32(output_stream, static_cast<std::uint32_t>(values.size()));
            for (const auto v: values)
            {
                write_uint32(output_stream, v);
            }
        }

        static void serialize_value_array(
            std::ostream&                                      output_stream,
            const std::function<std::string(const std::any&)>& value_serializer,
            const std::vector<std::any>&                       value_array)
        {
            assert(value_array.size() < std::numeric_limits<std::uint32_t>::max());
            write_uint32(output_stream, static_cast<std::uint32_t>(value_array.size()));
            for (const auto& v: value_array)
            {
                const auto serialized = value_serializer(v);
                assert(serialized.length() < std::numeric_limits<std::uint32_t>::max());
                write_uint32(output_stream, static_cast<std::uint32_t>(serialized.length()));
                output_stream.write(serialized.data(), serialized.length());
            }
        }

        static void write_uint32(std::ostream& output_stream, const std::uint32_t value)
        {
            static const default_serializer<std::uint32_t> uint32_serializer{};

            const auto serialized = uint32_serializer(value);
            output_stream.write(serialized.data(), serialized.length());
        }

        static void deserialize(
            std::istream&                                           input_stream,
            const std::function<std::any(const std::string_view&)>& value_deserializer,
            std::vector<std::uint32_t>&                             base_check_array,
            std::vector<std::size_t>&                               value_index_mappings,
            std::vector<std::any>&                                  value_array)
        {
            deserialize_base_check_array(input_stream, base_check_array);
            deserialize_value_index_mappings(input_stream, value_index_mappings);
            deserialize_value_array(input_stream, value_deserializer, value_array);
        }

        static void
        deserialize_base_check_array(std::istream& input_stream, std::vector<std::uint32_t>& base_check_array)
        {
            const auto size = read_uint32(input_stream);
            base_check_array.reserve(size);
            for (auto i = static_cast<std::uint32_t>(0); i < size; ++i)
            {
                base_check_array.push_back(read_uint32(input_stream));
            }
        }

        static void
        deserialize_value_index_mappings(std::istream& input_stream, std::vector<std::size_t>& value_index_mappings)
        {
            const auto size = read_uint32(input_stream);
            for (auto i = static_cast<std::uint32_t>(0); i < size; ++i)
            {
                const auto value = read_uint32(input_stream);
                if (value >= value_index_mappings.size())
                {
                    value_index_mappings.resize(value + 1, std::numeric_limits<std::size_t>::max());
                }
                value_index_mappings[value] = i;
            }
        }

        static void deserialize_value_array(
            std::istream&                                           input_stream,
            const std::function<std::any(const std::string_view&)>& value_deserializer,
            std::vector<std::any>&                                  value_array)
        {
            const auto size = read_uint32(input_stream);
            value_array.reserve(size);
            for (auto i = static_cast<std::uint32_t>(0); i < size; ++i)
            {
                const auto  element_size = read_uint32(input_stream);
                std::string to_deserialize(element_size, 0);
                input_stream.read(to_deserialize.data(), element_size);
                if (input_stream.gcount() < static_cast<std::streamsize>(element_size))
                {
                    throw std::ios_base::failure("Can't read value.");
                }
                value_array.push_back(value_deserializer(to_deserialize));
            }
        }

        static std::uint32_t read_uint32(std::istream& input_stream)
        {
            static const default_deserializer<std::uint32_t> uint32_deserializer{};

            std::string to_deserialize{};
            to_deserialize.reserve(sizeof(std::uint32_t));
            for (auto i = static_cast<std::size_t>(0); i < sizeof(std::uint32_t); ++i)
            {
                auto byte_ = static_cast<char>(0);
                input_stream.read(&byte_, sizeof(char));
                if (input_stream.gcount() < static_cast<std::streamsize>(sizeof(char)))
                {
                    throw std::ios_base::failure("Can't read uint32.");
                }
                to_deserialize.push_back(byte_);

                if (byte_ == static_cast<char>(0xFD))
                {
                    auto trailing_byte = static_cast<char>(0);
                    input_stream.read(&trailing_byte, sizeof(char));
                    if (input_stream.gcount() < static_cast<std::streamsize>(sizeof(char)))
                    {
                        throw std::ios_base::failure("Can't read uint32.");
                    }
                    to_deserialize.push_back(trailing_byte);
                }
            }
            return uint32_deserializer(to_deserialize);
        }


        // variables

        mutable std::vector<std::uint32_t> m_base_check_array;

        std::vector<std::size_t> m_value_index_mappings;

        std::vector<std::any> m_value_array;


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

    memory_storage::memory_storage(
        std::istream&                                           input_stream,
        const std::function<std::any(const std::string_view&)>& value_deserializer) :
    m_p_impl{ std::make_unique<impl>(input_stream, value_deserializer) }
    {}

    memory_storage::~memory_storage() = default;

    std::int32_t memory_storage::base_at_impl(const std::size_t base_check_index) const
    {
        return m_p_impl->base_at_impl(base_check_index);
    }

    void memory_storage::set_base_at_impl(const std::size_t base_check_index, const std::int32_t base)
    {
        m_p_impl->set_base_at_impl(base_check_index, base);
    }

    std::uint8_t memory_storage::check_at_impl(const std::size_t base_check_index) const
    {
        return m_p_impl->check_at_impl(base_check_index);
    }

    void memory_storage::set_check_at_impl(const std::size_t base_check_index, const std::uint8_t check)
    {
        m_p_impl->set_check_at_impl(base_check_index, check);
    }

    std::size_t memory_storage::size_impl() const
    {
        return m_p_impl->size_impl();
    }

    double memory_storage::filling_rate_impl() const
    {
        return m_p_impl->filling_rate_impl();
    }

    const std::vector<std::uint32_t>& memory_storage::base_check_array_impl() const
    {
        return m_p_impl->base_check_array_impl();
    }

    const std::any* memory_storage::value_at_impl(const std::size_t value_index) const
    {
        return m_p_impl->value_at_impl(value_index);
    }

    void memory_storage::add_value_at_impl(const std::size_t value_index, std::any value)
    {
        return m_p_impl->add_value_at_impl(value_index, std::move(value));
    }

    void memory_storage::serialize_impl(
        std::ostream&                                      output_stream,
        const std::function<std::string(const std::any&)>& value_serializer) const
    {
        m_p_impl->serialize_impl(output_stream, value_serializer);
    }

    std::unique_ptr<storage> memory_storage::clone_impl() const
    {
        return m_p_impl->clone_impl();
    }


}
