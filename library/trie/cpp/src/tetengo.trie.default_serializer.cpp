/*! \file
    \brief A default serializer.

    Copyright (C) 2019-2023 kaoru  https://www.tetengo.org/
*/

#include <cassert>
#include <cstddef>
#include <iterator>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include <tetengo/trie/default_serializer.hpp>


namespace tetengo::trie
{
#if !defined(DOCUMENTATION)
    namespace
    {
        template <typename T>
        std::vector<char> to_bytes_with_escape(const T value)
        {
            std::vector<char> bytes{};
            bytes.reserve(sizeof(T));
            for (auto i = static_cast<std::size_t>(0); i < sizeof(T); ++i)
            {
                if (const auto byte_ =
                        static_cast<char>(static_cast<unsigned char>(value >> (sizeof(T) - i - 1) * 8) & 0xFF);
                    byte_ == static_cast<char>(0x00))
                {
                    bytes.push_back(static_cast<char>(0xFE));
                }
                else if (byte_ == static_cast<char>(0xFD) || byte_ == static_cast<char>(0xFE))
                {
                    bytes.push_back(static_cast<char>(0xFD));
                    bytes.push_back(byte_);
                }
                else
                {
                    bytes.push_back(byte_);
                }
            }
            return bytes;
        }

        template <typename T>
        std::vector<char> to_bytes_without_escape(const T value)
        {
            std::vector<char> bytes{};
            bytes.reserve(sizeof(T));
            for (auto i = static_cast<std::size_t>(0); i < sizeof(T); ++i)
            {
                const auto byte_ =
                    static_cast<char>(static_cast<unsigned char>(value >> (sizeof(T) - i - 1) * 8) & 0xFF);
                bytes.push_back(byte_);
            }
            return bytes;
        }

        template <typename T>
        std::vector<char> to_bytes(const T value, const bool fe_escape)
        {
            return fe_escape ? to_bytes_with_escape(value) : to_bytes_without_escape(value);
        }

        template <typename T>
        T from_bytes_with_escape(const char* const p_head, const std::size_t size)
        {
            assert(sizeof(T) <= size && size <= 2 * sizeof(T));
            auto object = static_cast<T>(0);
            if constexpr (sizeof(T) > 1)
            {
                for (auto i = static_cast<std::size_t>(0); i < size; ++i)
                {
                    object <<= 8;

                    const auto byte_ = p_head[i];
                    auto       original_byte = byte_;
                    if (byte_ == static_cast<char>(0xFD))
                    {
                        if (i + 1 < size)
                        {
                            ++i;
                            if (const auto byte2 = p_head[i];
                                byte2 == static_cast<char>(0xFD) || byte2 == static_cast<char>(0xFE))
                            {
                                original_byte = byte2;
                            }
                            else
                            {
                                assert(false);
                            }
                        }
                        else
                        {
                            assert(false);
                        }
                    }
                    else if (byte_ == static_cast<char>(0xFE))
                    {
                        original_byte = static_cast<char>(0x00);
                    }
                    object |= static_cast<unsigned char>(original_byte);
                }
            }
            else
            {
                const auto byte_ = p_head[0];
                auto       original_byte = byte_;
                if (byte_ == static_cast<char>(0xFD))
                {
                    if (size > 1)
                    {
                        if (const auto byte2 = p_head[1];
                            byte2 == static_cast<char>(0xFD) || byte2 == static_cast<char>(0xFE))
                        {
                            original_byte = byte2;
                        }
                        else
                        {
                            assert(false);
                        }
                    }
                    else
                    {
                        assert(false);
                    }
                }
                else if (byte_ == static_cast<char>(0xFE))
                {
                    original_byte = static_cast<char>(0x00);
                }
                object = static_cast<unsigned char>(original_byte);
            }
            return object;
        }

        template <typename T>
        T from_bytes_without_escape(const char* const p_head, const std::size_t size)
        {
            assert(sizeof(T) <= size && size <= 2 * sizeof(T));
            auto object = static_cast<T>(0);
            if constexpr (sizeof(T) > 1)
            {
                for (auto i = static_cast<std::size_t>(0); i < size; ++i)
                {
                    object <<= 8;
                    const auto byte_ = p_head[i];
                    object |= static_cast<unsigned char>(byte_);
                }
            }
            else
            {
                object = *p_head;
            }
            return object;
        }

        template <typename T>
        T from_bytes(const char* const p_head, const std::size_t size, const bool fe_escape)
        {
            return fe_escape ? from_bytes_with_escape<T>(p_head, size) : from_bytes_without_escape<T>(p_head, size);
        }


    }


    default_serializer<std::string_view>::default_serializer(const bool /*ignored*/) {}

    const std::string_view& default_serializer<std::string_view>::operator()(const std::string_view& object) const
    {
        return object;
    }


    default_serializer<std::string>::default_serializer(const bool /*ignored*/) {}

    const std::string& default_serializer<std::string>::operator()(const std::string& object) const
    {
        return object;
    }


    template <typename Char>
    default_serializer<std::basic_string_view<Char>>::default_serializer(const bool fe_escape) :
    m_fe_escape{ fe_escape }
    {}

    template default_serializer<std::basic_string_view<wchar_t>>::default_serializer(const bool fe_escape);

    template default_serializer<std::basic_string_view<char16_t>>::default_serializer(const bool fe_escape);

    template default_serializer<std::basic_string_view<char32_t>>::default_serializer(const bool fe_escape);

    template <typename Char>
    std::vector<char>
    default_serializer<std::basic_string_view<Char>>::operator()(const std::basic_string_view<Char>& object) const
    {
        std::vector<char> serialized{};
        serialized.reserve(object.length() * sizeof(Char) / sizeof(char));
        for (const auto c: object)
        {
            const auto bytes = to_bytes(c, m_fe_escape);
            serialized.insert(std::end(serialized), std::begin(bytes), std::end(bytes));
        }
        return serialized;
    }

    template std::vector<char> default_serializer<std::basic_string_view<wchar_t>>::operator()(
        const std::basic_string_view<wchar_t>& object) const;

    template std::vector<char> default_serializer<std::basic_string_view<char16_t>>::operator()(
        const std::basic_string_view<char16_t>& object) const;

    template std::vector<char> default_serializer<std::basic_string_view<char32_t>>::operator()(
        const std::basic_string_view<char32_t>& object) const;


    template <typename Char>
    default_serializer<std::basic_string<Char>>::default_serializer(const bool fe_escape) : m_fe_escape{ fe_escape }
    {}

    template default_serializer<std::basic_string<wchar_t>>::default_serializer(const bool fe_escape);

    template default_serializer<std::basic_string<char16_t>>::default_serializer(const bool fe_escape);

    template default_serializer<std::basic_string<char32_t>>::default_serializer(const bool fe_escape);

    template <typename Char>
    std::vector<char>
    default_serializer<std::basic_string<Char>>::operator()(const std::basic_string<Char>& object) const
    {
        std::vector<char> serialized{};
        serialized.reserve(object.length() * sizeof(Char) / sizeof(char));
        for (const auto c: object)
        {
            const auto bytes = to_bytes(c, m_fe_escape);
            serialized.insert(std::end(serialized), std::begin(bytes), std::end(bytes));
        }
        return serialized;
    }

    template std::vector<char>
    default_serializer<std::basic_string<wchar_t>>::operator()(const std::basic_string<wchar_t>& object) const;

    template std::vector<char>
    default_serializer<std::basic_string<char16_t>>::operator()(const std::basic_string<char16_t>& object) const;

    template std::vector<char>
    default_serializer<std::basic_string<char32_t>>::operator()(const std::basic_string<char32_t>& object) const;


    template <typename Integer>
    default_serializer<Integer, std::enable_if_t<std::is_integral_v<Integer>>>::default_serializer(
        const bool fe_escape) :
    m_fe_escape{ fe_escape }
    {}

    template default_serializer<bool, std::enable_if_t<std::is_integral_v<bool>>>::default_serializer(
        const bool fe_escape);

    template default_serializer<char, std::enable_if_t<std::is_integral_v<char>>>::default_serializer(
        const bool fe_escape);

    template default_serializer<signed char, std::enable_if_t<std::is_integral_v<signed char>>>::default_serializer(
        const bool fe_escape);

    template default_serializer<unsigned char, std::enable_if_t<std::is_integral_v<unsigned char>>>::default_serializer(
        const bool fe_escape);

    template default_serializer<wchar_t, std::enable_if_t<std::is_integral_v<wchar_t>>>::default_serializer(
        const bool fe_escape);

    template default_serializer<char16_t, std::enable_if_t<std::is_integral_v<char16_t>>>::default_serializer(
        const bool fe_escape);

    template default_serializer<char32_t, std::enable_if_t<std::is_integral_v<char32_t>>>::default_serializer(
        const bool fe_escape);

    template default_serializer<int, std::enable_if_t<std::is_integral_v<int>>>::default_serializer(
        const bool fe_escape);

    template default_serializer<unsigned int, std::enable_if_t<std::is_integral_v<unsigned int>>>::default_serializer(
        const bool fe_escape);

    template default_serializer<short, std::enable_if_t<std::is_integral_v<short>>>::default_serializer(
        const bool fe_escape);

    template default_serializer<unsigned short, std::enable_if_t<std::is_integral_v<unsigned short>>>::
        default_serializer(const bool fe_escape);

    template default_serializer<long, std::enable_if_t<std::is_integral_v<long>>>::default_serializer(
        const bool fe_escape);

    template default_serializer<unsigned long, std::enable_if_t<std::is_integral_v<unsigned long>>>::default_serializer(
        const bool fe_escape);

    template default_serializer<long long, std::enable_if_t<std::is_integral_v<long long>>>::default_serializer(
        const bool fe_escape);

    template default_serializer<unsigned long long, std::enable_if_t<std::is_integral_v<unsigned long long>>>::
        default_serializer(const bool fe_escape);

    template <typename Integer>
    std::vector<char>
    default_serializer<Integer, std::enable_if_t<std::is_integral_v<Integer>>>::operator()(const Integer object) const
    {
        return to_bytes(object, m_fe_escape);
    }

    template std::vector<char>
    default_serializer<bool, std::enable_if_t<std::is_integral_v<bool>>>::operator()(bool object) const;

    template std::vector<char>
    default_serializer<char, std::enable_if_t<std::is_integral_v<char>>>::operator()(char object) const;

    template std::vector<char>
    default_serializer<signed char, std::enable_if_t<std::is_integral_v<signed char>>>::operator()(
        signed char object) const;

    template std::vector<char>
    default_serializer<unsigned char, std::enable_if_t<std::is_integral_v<unsigned char>>>::operator()(
        unsigned char object) const;

    template std::vector<char>
    default_serializer<wchar_t, std::enable_if_t<std::is_integral_v<wchar_t>>>::operator()(wchar_t object) const;

    template std::vector<char>
    default_serializer<char16_t, std::enable_if_t<std::is_integral_v<char16_t>>>::operator()(char16_t object) const;

    template std::vector<char>
    default_serializer<char32_t, std::enable_if_t<std::is_integral_v<char32_t>>>::operator()(char32_t object) const;

    template std::vector<char>
    default_serializer<int, std::enable_if_t<std::is_integral_v<int>>>::operator()(int object) const;

    template std::vector<char>
    default_serializer<unsigned int, std::enable_if_t<std::is_integral_v<unsigned int>>>::operator()(
        unsigned int object) const;

    template std::vector<char>
    default_serializer<short, std::enable_if_t<std::is_integral_v<short>>>::operator()(short object) const;

    template std::vector<char>
    default_serializer<unsigned short, std::enable_if_t<std::is_integral_v<unsigned short>>>::operator()(
        unsigned short object) const;

    template std::vector<char>
    default_serializer<long, std::enable_if_t<std::is_integral_v<long>>>::operator()(long object) const;

    template std::vector<char>
    default_serializer<unsigned long, std::enable_if_t<std::is_integral_v<unsigned long>>>::operator()(
        unsigned long object) const;

    template std::vector<char>
    default_serializer<long long, std::enable_if_t<std::is_integral_v<long long>>>::operator()(long long object) const;

    template std::vector<char>
    default_serializer<unsigned long long, std::enable_if_t<std::is_integral_v<unsigned long long>>>::operator()(
        unsigned long long object) const;


    default_deserializer<std::string>::default_deserializer(const bool /*ignored*/) {}

    const std::string& default_deserializer<std::string>::operator()(const std::string& bytes) const
    {
        return bytes;
    }


    template <typename Char>
    default_deserializer<std::basic_string<Char>>::default_deserializer(const bool fe_escape) : m_fe_escape{ fe_escape }
    {}

    template default_deserializer<std::basic_string<wchar_t>>::default_deserializer(const bool fe_escape);

    template default_deserializer<std::basic_string<char16_t>>::default_deserializer(const bool fe_escape);

    template default_deserializer<std::basic_string<char32_t>>::default_deserializer(const bool fe_escape);

    template <typename Char>
    std::basic_string<Char>
    default_deserializer<std::basic_string<Char>>::operator()(const std::vector<char>& bytes) const
    {
        assert(std::size(bytes) % sizeof(Char) == 0);
        std::basic_string<Char> object{};
        for (auto i = std::begin(bytes); i != std::end(bytes); i += sizeof(Char))
        {
            object.push_back(from_bytes<Char>(&*i, sizeof(Char), m_fe_escape));
        }
        return object;
    }

    template std::basic_string<wchar_t>
    default_deserializer<std::basic_string<wchar_t>>::operator()(const std::vector<char>& bytes) const;

    template std::basic_string<char16_t>
    default_deserializer<std::basic_string<char16_t>>::operator()(const std::vector<char>& bytes) const;

    template std::basic_string<char32_t>
    default_deserializer<std::basic_string<char32_t>>::operator()(const std::vector<char>& bytes) const;


    template <typename Integer>
    default_deserializer<Integer, std::enable_if_t<std::is_integral_v<Integer>>>::default_deserializer(
        const bool fe_escape) :
    m_fe_escape{ fe_escape }
    {}

    template default_deserializer<bool, std::enable_if_t<std::is_integral_v<bool>>>::default_deserializer(
        const bool fe_escape);

    template default_deserializer<char, std::enable_if_t<std::is_integral_v<char>>>::default_deserializer(
        const bool fe_escape);

    template default_deserializer<signed char, std::enable_if_t<std::is_integral_v<signed char>>>::default_deserializer(
        const bool fe_escape);

    template default_deserializer<unsigned char, std::enable_if_t<std::is_integral_v<unsigned char>>>::
        default_deserializer(const bool fe_escape);

    template default_deserializer<wchar_t, std::enable_if_t<std::is_integral_v<wchar_t>>>::default_deserializer(
        const bool fe_escape);

    template default_deserializer<char16_t, std::enable_if_t<std::is_integral_v<char16_t>>>::default_deserializer(
        const bool fe_escape);

    template default_deserializer<char32_t, std::enable_if_t<std::is_integral_v<char32_t>>>::default_deserializer(
        const bool fe_escape);

    template default_deserializer<int, std::enable_if_t<std::is_integral_v<int>>>::default_deserializer(
        const bool fe_escape);

    template default_deserializer<unsigned int, std::enable_if_t<std::is_integral_v<unsigned int>>>::
        default_deserializer(const bool fe_escape);

    template default_deserializer<short, std::enable_if_t<std::is_integral_v<short>>>::default_deserializer(
        const bool fe_escape);

    template default_deserializer<unsigned short, std::enable_if_t<std::is_integral_v<unsigned short>>>::
        default_deserializer(const bool fe_escape);

    template default_deserializer<long, std::enable_if_t<std::is_integral_v<long>>>::default_deserializer(
        const bool fe_escape);

    template default_deserializer<unsigned long, std::enable_if_t<std::is_integral_v<unsigned long>>>::
        default_deserializer(const bool fe_escape);

    template default_deserializer<long long, std::enable_if_t<std::is_integral_v<long long>>>::default_deserializer(
        const bool fe_escape);

    template default_deserializer<unsigned long long, std::enable_if_t<std::is_integral_v<unsigned long long>>>::
        default_deserializer(const bool fe_escape);

    template <typename Integer>
    Integer default_deserializer<Integer, std::enable_if_t<std::is_integral_v<Integer>>>::operator()(
        const std::vector<char>& bytes) const
    {
        return from_bytes<Integer>(std::data(bytes), std::size(bytes), m_fe_escape);
    }

    template bool default_deserializer<bool, std::enable_if_t<std::is_integral_v<bool>>>::operator()(
        const std::vector<char>& bytes) const;

    template char default_deserializer<char, std::enable_if_t<std::is_integral_v<char>>>::operator()(
        const std::vector<char>& bytes) const;

    template signed char
    default_deserializer<signed char, std::enable_if_t<std::is_integral_v<signed char>>>::operator()(
        const std::vector<char>& bytes) const;

    template unsigned char
    default_deserializer<unsigned char, std::enable_if_t<std::is_integral_v<unsigned char>>>::operator()(
        const std::vector<char>& bytes) const;

    template wchar_t default_deserializer<wchar_t, std::enable_if_t<std::is_integral_v<wchar_t>>>::operator()(
        const std::vector<char>& bytes) const;

    template char16_t default_deserializer<char16_t, std::enable_if_t<std::is_integral_v<char16_t>>>::operator()(
        const std::vector<char>& bytes) const;

    template char32_t default_deserializer<char32_t, std::enable_if_t<std::is_integral_v<char32_t>>>::operator()(
        const std::vector<char>& bytes) const;

    template int default_deserializer<int, std::enable_if_t<std::is_integral_v<int>>>::operator()(
        const std::vector<char>& bytes) const;

    template unsigned int
    default_deserializer<unsigned int, std::enable_if_t<std::is_integral_v<unsigned int>>>::operator()(
        const std::vector<char>& bytes) const;

    template short default_deserializer<short, std::enable_if_t<std::is_integral_v<short>>>::operator()(
        const std::vector<char>& bytes) const;

    template unsigned short
    default_deserializer<unsigned short, std::enable_if_t<std::is_integral_v<unsigned short>>>::operator()(
        const std::vector<char>& bytes) const;

    template long default_deserializer<long, std::enable_if_t<std::is_integral_v<long>>>::operator()(
        const std::vector<char>& bytes) const;

    template unsigned long
    default_deserializer<unsigned long, std::enable_if_t<std::is_integral_v<unsigned long>>>::operator()(
        const std::vector<char>& bytes) const;

    template long long default_deserializer<long long, std::enable_if_t<std::is_integral_v<long long>>>::operator()(
        const std::vector<char>& bytes) const;

    template unsigned long long
    default_deserializer<unsigned long long, std::enable_if_t<std::is_integral_v<unsigned long long>>>::operator()(
        const std::vector<char>& bytes) const;
#endif


}
