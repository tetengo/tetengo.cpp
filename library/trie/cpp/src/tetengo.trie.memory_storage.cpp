/*! \file
    \brief A memory storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <any>
#include <cassert>
#include <cstdint>
#include <istream>
#include <iterator>
#include <limits>
#include <memory>
#include <optional>
#include <type_traits> // IWYU pragma: keep
#include <utility>
#include <vector>

#include <boost/core/noncopyable.hpp>

#include <tetengo/trie/default_serializer.hpp>
#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/memory_storage.hpp>
#include <tetengo/trie/storage.hpp>
#include <tetengo/trie/value_serializer.hpp>


namespace tetengo::trie
{
    class memory_storage::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        impl() : m_base_check_array{ 0x00000000U | double_array::vacant_check_value() }, m_value_array{} {};

        explicit impl(std::istream& input_stream, const value_deserializer& value_deserializer_) :
        m_base_check_array{},
        m_value_array{}
        {
            deserialize(input_stream, value_deserializer_, m_base_check_array, m_value_array);
        };


        // functions

        std::size_t base_check_size_impl() const
        {
            return std::size(m_base_check_array);
        }

        std::int32_t base_at_impl(const std::size_t base_check_index) const
        {
            ensure_base_check_size(base_check_index + 1);
            return static_cast<std::int32_t>(m_base_check_array[base_check_index]) >> 8;
        }

        void set_base_at_impl(const std::size_t base_check_index, const std::int32_t base)
        {
            ensure_base_check_size(base_check_index + 1);
            m_base_check_array[base_check_index] &= 0x000000FF;
            m_base_check_array[base_check_index] |= static_cast<std::uint32_t>(base << 8);
        }

        std::uint8_t check_at_impl(const std::size_t base_check_index) const
        {
            ensure_base_check_size(base_check_index + 1);
            return m_base_check_array[base_check_index] & 0xFF;
        }

        void set_check_at_impl(const std::size_t base_check_index, const std::uint8_t check)
        {
            ensure_base_check_size(base_check_index + 1);
            m_base_check_array[base_check_index] &= 0xFFFFFF00;
            m_base_check_array[base_check_index] |= check;
        }

        std::size_t value_count_impl() const
        {
            return std::size(m_value_array);
        }

        const std::any* value_at_impl(const std::size_t value_index) const
        {
            if (value_index >= std::size(m_value_array) || !m_value_array[value_index])
            {
                return nullptr;
            }
            return &*m_value_array[value_index];
        }

        void add_value_at_impl(const std::size_t value_index, std::any value)
        {
            if (value_index >= std::size(m_value_array))
            {
                m_value_array.resize(value_index + 1, std::nullopt);
            }
            m_value_array[value_index] = std::move(value);
        }

        double filling_rate_impl() const
        {
            const auto empty_count =
                std::count(std::begin(m_base_check_array), std::end(m_base_check_array), 0x000000FFU);
            return 1.0 - static_cast<double>(empty_count) / std::size(m_base_check_array);
        }

        void serialize_impl(std::ostream& output_stream, const value_serializer& value_serializer_) const
        {
            if (!output_stream)
            {
                throw std::ios_base::failure{ "Bad output_stream." };
            }

            serialize_base_check_array(output_stream, m_base_check_array);
            serialize_value_array(output_stream, value_serializer_, m_value_array);
        }

        std::unique_ptr<storage> clone_impl() const
        {
            auto p_clone = std::make_unique<memory_storage>();
            p_clone->m_p_impl->m_base_check_array = m_base_check_array;
            p_clone->m_p_impl->m_value_array = m_value_array;
            return p_clone;
        }


    private:
        // static functions

        static void
        serialize_base_check_array(std::ostream& output_stream, const std::vector<std::uint32_t>& base_check_array)
        {
            assert(std::size(base_check_array) < std::numeric_limits<std::uint32_t>::max());
            write_uint32(output_stream, static_cast<std::uint32_t>(std::size(base_check_array)));
            for (const auto v: base_check_array)
            {
                write_uint32(output_stream, v);
            }
        }

        static void serialize_value_array(
            std::ostream&                               output_stream,
            const value_serializer&                     value_serializer_,
            const std::vector<std::optional<std::any>>& value_array)
        {
            assert(std::size(value_array) < std::numeric_limits<std::uint32_t>::max());
            write_uint32(output_stream, static_cast<std::uint32_t>(std::size(value_array)));

            assert(value_serializer_.fixed_value_size() < std::numeric_limits<std::uint32_t>::max());
            const auto fixed_value_size = static_cast<std::uint32_t>(value_serializer_.fixed_value_size());
            write_uint32(output_stream, fixed_value_size);

            if (fixed_value_size == 0)
            {
                for (const auto& v: value_array)
                {
                    if (v)
                    {
                        const auto serialized = value_serializer_(*v);
                        assert(std::size(serialized) < std::numeric_limits<std::uint32_t>::max());
                        write_uint32(output_stream, static_cast<std::uint32_t>(std::size(serialized)));
                        output_stream.write(std::data(serialized), std::size(serialized));
                    }
                    else
                    {
                        write_uint32(output_stream, 0);
                    }
                }
            }
            else
            {
                for (const auto& v: value_array)
                {
                    if (v)
                    {
                        const auto serialized = value_serializer_(*v);
                        assert(std::size(serialized) == fixed_value_size);
                        output_stream.write(std::data(serialized), fixed_value_size);
                    }
                    else
                    {
                        std::vector<char> uninitialized(fixed_value_size, uninitialized_byte());
                        output_stream.write(std::data(uninitialized), fixed_value_size);
                    }
                }
            }
        }

        static void write_uint32(std::ostream& output_stream, const std::uint32_t value)
        {
            static const default_serializer<std::uint32_t> uint32_serializer{ false };

            const auto serialized = uint32_serializer(value);
            output_stream.write(std::data(serialized), std::size(serialized));
        }

        static void deserialize(
            std::istream&                         input_stream,
            const value_deserializer&             value_deserializer_,
            std::vector<std::uint32_t>&           base_check_array,
            std::vector<std::optional<std::any>>& value_array)
        {
            deserialize_base_check_array(input_stream, base_check_array);
            deserialize_value_array(input_stream, value_deserializer_, value_array);
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

        static void deserialize_value_array(
            std::istream&                         input_stream,
            const value_deserializer&             value_deserializer_,
            std::vector<std::optional<std::any>>& value_array)
        {
            const auto size = read_uint32(input_stream);
            value_array.reserve(size);

            const auto fixed_value_size = read_uint32(input_stream);

            if (fixed_value_size == 0)
            {
                for (auto i = static_cast<std::uint32_t>(0); i < size; ++i)
                {
                    const auto element_size = read_uint32(input_stream);
                    if (element_size > 0)
                    {
                        std::vector<char> to_deserialize(element_size, 0);
                        input_stream.read(std::data(to_deserialize), element_size);
                        if (input_stream.gcount() < static_cast<std::streamsize>(element_size))
                        {
                            throw std::ios_base::failure("Can't read value.");
                        }
                        value_array.push_back(value_deserializer_(to_deserialize));
                    }
                    else
                    {
                        value_array.push_back(std::nullopt);
                    }
                }
            }
            else
            {
                for (auto i = static_cast<std::uint32_t>(0); i < size; ++i)
                {
                    std::vector<char> to_deserialize(fixed_value_size, 0);
                    input_stream.read(std::data(to_deserialize), fixed_value_size);
                    if (input_stream.gcount() < static_cast<std::streamsize>(fixed_value_size))
                    {
                        throw std::ios_base::failure("Can't read value.");
                    }
                    if (std::all_of(std::begin(to_deserialize), std::end(to_deserialize), [](const auto e) {
                            return e == uninitialized_byte();
                        }))
                    {
                        value_array.push_back(std::nullopt);
                    }
                    else
                    {
                        value_array.push_back(value_deserializer_(to_deserialize));
                    }
                }
            }
        }

        static std::uint32_t read_uint32(std::istream& input_stream)
        {
            static const default_deserializer<std::uint32_t> uint32_deserializer{ false };

            std::vector<char> to_deserialize{};
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
            }
            return uint32_deserializer(to_deserialize);
        }

        static constexpr char uninitialized_byte()
        {
            return static_cast<char>(0xFF);
        }


        // variables

        mutable std::vector<std::uint32_t> m_base_check_array;

        std::vector<std::optional<std::any>> m_value_array;


        // functions

        void ensure_base_check_size(const std::size_t size) const
        {
            if (size > std::size(m_base_check_array))
            {
                m_base_check_array.resize(size, 0x00000000U | double_array::vacant_check_value());
            }
        }
    };


    memory_storage::memory_storage() : m_p_impl{ std::make_unique<impl>() } {}

    memory_storage::memory_storage(std::istream& input_stream, const value_deserializer& value_deserializer_) :
    m_p_impl{ std::make_unique<impl>(input_stream, value_deserializer_) }
    {}

    memory_storage::~memory_storage() = default;

    std::size_t memory_storage::base_check_size_impl() const
    {
        return m_p_impl->base_check_size_impl();
    }

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

    std::size_t memory_storage::value_count_impl() const
    {
        return m_p_impl->value_count_impl();
    }

    const std::any* memory_storage::value_at_impl(const std::size_t value_index) const
    {
        return m_p_impl->value_at_impl(value_index);
    }

    void memory_storage::add_value_at_impl(const std::size_t value_index, std::any value)
    {
        return m_p_impl->add_value_at_impl(value_index, std::move(value));
    }

    double memory_storage::filling_rate_impl() const
    {
        return m_p_impl->filling_rate_impl();
    }

    void memory_storage::serialize_impl(std::ostream& output_stream, const value_serializer& value_serializer_) const
    {
        m_p_impl->serialize_impl(output_stream, value_serializer_);
    }

    std::unique_ptr<storage> memory_storage::clone_impl() const
    {
        return m_p_impl->clone_impl();
    }


}
