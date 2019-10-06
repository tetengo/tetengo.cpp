/*! \file
    \brief A storage.

    Copyright (C) 2019 kaoru
*/

#include <algorithm>
#include <array>
#include <cstdint>
#include <istream>
#include <vector>

#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/storage.hpp>


namespace tetengo::trie
{
    namespace
    {
        std::uint32_t read_uint32(std::istream& input_stream)
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

        void write_uint32(std::ostream& output_stream, const std::uint32_t value)
        {
            const std::array<char, 4> buffer{ static_cast<char>((value & 0xFF000000) >> 24),
                                              static_cast<char>((value & 0x00FF0000) >> 16),
                                              static_cast<char>((value & 0x0000FF00) >> 8),
                                              static_cast<char>(value & 0x00FF00FF) };
            output_stream.write(buffer.data(), buffer.size());
        }

        std::vector<std::uint32_t> deserialize(std::istream& input_stream)
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

        void ensure_size(std::vector<std::uint32_t>& values, const std::size_t size)
        {
            if (size > values.size())
            {
                values.resize(size, 0x00000000U | double_array::vacant_check_value());
            }
        }


    }


    storage::storage() : m_values{ 0x00000000U | double_array::vacant_check_value() } {}

    storage::storage(std::istream& input_stream) : m_values{ deserialize(input_stream) } {}

    std::int32_t storage::base_at(const std::size_t index) const
    {
        ensure_size(m_values, index + 1);
        return static_cast<std::int32_t>(m_values[index]) >> 8;
    }

    void storage::set_base_at(const std::size_t index, const std::int32_t value)
    {
        ensure_size(m_values, index + 1);
        m_values[index] &= 0x000000FF;
        m_values[index] |= static_cast<std::uint32_t>(value << 8);
    }

    std::uint8_t storage::check_at(const std::size_t index) const
    {
        ensure_size(m_values, index + 1);
        return m_values[index] & 0xFF;
    }

    void storage::set_check_at(const std::size_t index, const std::uint8_t value)
    {
        ensure_size(m_values, index + 1);
        m_values[index] &= 0xFFFFFF00;
        m_values[index] |= value;
    }

    std::size_t storage::size() const
    {
        return m_values.size();
    }

    double storage::filling_rate() const
    {
        const auto empty_count = std::count(m_values.begin(), m_values.end(), 0x000000FFU);
        return 1.0 - static_cast<double>(empty_count) / m_values.size();
    }

    const std::vector<std::uint32_t>& storage::values() const
    {
        return m_values;
    }

    void storage::serialize(std::ostream& output_stream) const
    {
        write_uint32(output_stream, static_cast<std::uint32_t>(m_values.size()));
        for (const auto& v: m_values)
        {
            write_uint32(output_stream, v);
        }
    }


}
