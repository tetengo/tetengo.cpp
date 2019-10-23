/*! \file
    \brief A memory storage.

    Copyright (C) 2019 kaoru
*/

#include <algorithm>
#include <array>
#include <cstdint>
#include <istream>
#include <memory>
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

        impl() : m_values{ 0x00000000U | double_array::vacant_check_value() } {};

        explicit impl(std::istream& input_stream) : m_values{ deserialize(input_stream) } {};


        // functions

        std::int32_t base_at_impl(const std::size_t index) const
        {
            ensure_size(index + 1);
            return static_cast<std::int32_t>(m_values[index]) >> 8;
        }

        void set_base_at_impl(const std::size_t index, const std::int32_t value)
        {
            ensure_size(index + 1);
            m_values[index] &= 0x000000FF;
            m_values[index] |= static_cast<std::uint32_t>(value << 8);
        }

        std::uint8_t check_at_impl(const std::size_t index) const
        {
            ensure_size(index + 1);
            return m_values[index] & 0xFF;
        }

        void set_check_at_impl(const std::size_t index, const std::uint8_t value)
        {
            ensure_size(index + 1);
            m_values[index] &= 0xFFFFFF00;
            m_values[index] |= value;
        }

        std::size_t size_impl() const
        {
            return m_values.size();
        }

        double filling_rate_impl() const
        {
            const auto empty_count = std::count(m_values.begin(), m_values.end(), 0x000000FFU);
            return 1.0 - static_cast<double>(empty_count) / m_values.size();
        }

        const std::vector<std::uint32_t>& values_impl() const
        {
            return m_values;
        }

        void serialize_impl(std::ostream& output_stream) const
        {
            write_uint32(output_stream, static_cast<std::uint32_t>(m_values.size()));
            for (const auto& v: m_values)
            {
                write_uint32(output_stream, v);
            }
        }

        std::unique_ptr<storage> clone_impl() const
        {
            auto p_clone = std::make_unique<memory_storage>();
            p_clone->m_p_impl->m_values = m_values;
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

        static std::vector<std::uint32_t> deserialize(std::istream& input_stream)
        {
            const auto size = read_uint32(input_stream);

            std::vector<std::uint32_t> values;
            values.reserve(size);

            for (auto i = static_cast<std::uint32_t>(0); i < size; ++i)
            {
                values.push_back(read_uint32(input_stream));
            }

            return values;
        }


        // variables

        mutable std::vector<std::uint32_t> m_values;


        // functions

        void ensure_size(const std::size_t size) const
        {
            if (size > m_values.size())
            {
                m_values.resize(size, 0x00000000U | double_array::vacant_check_value());
            }
        }
    };


    memory_storage::memory_storage() : m_p_impl{ std::make_unique<impl>() } {}

    memory_storage::memory_storage(std::istream& input_stream) : m_p_impl{ std::make_unique<impl>(input_stream) } {}

    memory_storage::~memory_storage() = default;

    std::int32_t memory_storage::base_at_impl(const std::size_t index) const
    {
        return m_p_impl->base_at_impl(index);
    }

    void memory_storage::set_base_at_impl(const std::size_t index, const std::int32_t value)
    {
        m_p_impl->set_base_at_impl(index, value);
    }

    std::uint8_t memory_storage::check_at_impl(const std::size_t index) const
    {
        return m_p_impl->check_at_impl(index);
    }

    void memory_storage::set_check_at_impl(const std::size_t index, const std::uint8_t value)
    {
        m_p_impl->set_check_at_impl(index, value);
    }

    std::size_t memory_storage::size_impl() const
    {
        return m_p_impl->size_impl();
    }

    double memory_storage::filling_rate_impl() const
    {
        return m_p_impl->filling_rate_impl();
    }

    const std::vector<std::uint32_t>& memory_storage::values_impl() const
    {
        return m_p_impl->values_impl();
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
