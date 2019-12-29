/*! \file
    \brief A default serializer.

    Copyright (C) 2019 kaoru
*/

#include <cassert>
#include <cstddef>
#include <string>
#include <string_view>
#include <type_traits>

#include <tetengo/trie/default_serializer.hpp>


namespace tetengo::trie
{
#if !defined(DOCUMENTATION)
    namespace
    {
        template <typename T>
        std::string to_bytes(const T value)
        {
            std::string bytes{};
            bytes.reserve(sizeof(T));
            for (auto i = static_cast<std::size_t>(0); i < sizeof(T); ++i)
            {
                const auto byte =
                    static_cast<char>(static_cast<unsigned char>(value >> (sizeof(T) - i - 1) * 8) & 0xFF);
                bytes.push_back(byte);
            }
            return bytes;
        }

        template <typename T>
        T from_bytes(const std::string_view& bytes)
        {
            assert(bytes.length() == sizeof(T));
            auto object = static_cast<T>(0);
            if constexpr (sizeof(T) > 1)
            {
                for (const auto b: bytes)
                {
                    object <<= 8;
                    object |= static_cast<unsigned char>(b);
                }
            }
            else
            {
                object = static_cast<unsigned char>(bytes[0]);
            }
            return object;
        }

    }


    const std::string& default_serializer<std::string>::operator()(const std::string& object) const
    {
        return object;
    }


    template <typename Char>
    std::string default_serializer<std::basic_string<Char>>::operator()(const std::basic_string<Char>& object) const
    {
        std::string serialized;
        serialized.reserve(object.length() * sizeof(Char) / sizeof(char));
        for (const auto c: object)
        {
            serialized += to_bytes(c);
        }
        return serialized;
    }

    template std::string default_serializer<std::basic_string<wchar_t>>::
                         operator()(const std::basic_string<wchar_t>& object) const;

    template std::string default_serializer<std::basic_string<char16_t>>::
                         operator()(const std::basic_string<char16_t>& object) const;

    template std::string default_serializer<std::basic_string<char32_t>>::
                         operator()(const std::basic_string<char32_t>& object) const;


    template <typename Integer>
    std::string default_serializer<Integer, std::enable_if_t<std::is_integral_v<Integer>>>::
                operator()(const Integer object) const
    {
        return to_bytes(object);
    }

    template std::string default_serializer<bool, std::enable_if_t<std::is_integral_v<bool>>>::
                         operator()(bool object) const;

    template std::string default_serializer<char, std::enable_if_t<std::is_integral_v<char>>>::
                         operator()(char object) const;

    template std::string default_serializer<signed char, std::enable_if_t<std::is_integral_v<signed char>>>::
                         operator()(signed char object) const;

    template std::string default_serializer<unsigned char, std::enable_if_t<std::is_integral_v<unsigned char>>>::
                         operator()(unsigned char object) const;

    template std::string default_serializer<wchar_t, std::enable_if_t<std::is_integral_v<wchar_t>>>::
                         operator()(wchar_t object) const;

    template std::string default_serializer<char16_t, std::enable_if_t<std::is_integral_v<char16_t>>>::
                         operator()(char16_t object) const;

    template std::string default_serializer<char32_t, std::enable_if_t<std::is_integral_v<char32_t>>>::
                         operator()(char32_t object) const;

    template std::string default_serializer<int, std::enable_if_t<std::is_integral_v<int>>>::
                         operator()(int object) const;

    template std::string default_serializer<unsigned int, std::enable_if_t<std::is_integral_v<unsigned int>>>::
                         operator()(unsigned int object) const;

    template std::string default_serializer<short, std::enable_if_t<std::is_integral_v<short>>>::
                         operator()(short object) const;

    template std::string default_serializer<unsigned short, std::enable_if_t<std::is_integral_v<unsigned short>>>::
                         operator()(unsigned short object) const;

    template std::string default_serializer<long, std::enable_if_t<std::is_integral_v<long>>>::
                         operator()(long object) const;

    template std::string default_serializer<unsigned long, std::enable_if_t<std::is_integral_v<unsigned long>>>::
                         operator()(unsigned long object) const;

    template std::string default_serializer<long long, std::enable_if_t<std::is_integral_v<long long>>>::
                         operator()(long long object) const;

    template std::string
    default_serializer<unsigned long long, std::enable_if_t<std::is_integral_v<unsigned long long>>>::
    operator()(unsigned long long object) const;


    const std::string& default_deserializer<std::string>::operator()(const std::string& bytes) const
    {
        return bytes;
    }


    template <typename Char>
    std::basic_string<Char> default_deserializer<std::basic_string<Char>>::
                            operator()(const std::string_view& bytes) const
    {
        assert(bytes.length() % sizeof(Char) == 0);
        std::basic_string<Char> object{};
        for (auto i = bytes.begin(); i != bytes.end(); i += sizeof(Char))
        {
            object.push_back(from_bytes<Char>(std::string_view{ &*i, sizeof(Char) }));
        }
        return object;
    }

    template std::basic_string<wchar_t> default_deserializer<std::basic_string<wchar_t>>::
                                        operator()(const std::string_view& bytes) const;

    template std::basic_string<char16_t> default_deserializer<std::basic_string<char16_t>>::
                                         operator()(const std::string_view& bytes) const;

    template std::basic_string<char32_t> default_deserializer<std::basic_string<char32_t>>::
                                         operator()(const std::string_view& bytes) const;


    template <typename Integer>
    Integer default_deserializer<Integer, std::enable_if_t<std::is_integral_v<Integer>>>::
            operator()(const std::string_view& bytes) const
    {
        return from_bytes<Integer>(bytes);
    }

    template bool default_deserializer<bool, std::enable_if_t<std::is_integral_v<bool>>>::
                  operator()(const std::string_view& bytes) const;

    template char default_deserializer<char, std::enable_if_t<std::is_integral_v<char>>>::
                  operator()(const std::string_view& bytes) const;

    template signed char default_deserializer<signed char, std::enable_if_t<std::is_integral_v<signed char>>>::
                         operator()(const std::string_view& bytes) const;

    template unsigned char default_deserializer<unsigned char, std::enable_if_t<std::is_integral_v<unsigned char>>>::
                           operator()(const std::string_view& bytes) const;

    template wchar_t default_deserializer<wchar_t, std::enable_if_t<std::is_integral_v<wchar_t>>>::
                     operator()(const std::string_view& bytes) const;

    template char16_t default_deserializer<char16_t, std::enable_if_t<std::is_integral_v<char16_t>>>::
                      operator()(const std::string_view& bytes) const;

    template char32_t default_deserializer<char32_t, std::enable_if_t<std::is_integral_v<char32_t>>>::
                      operator()(const std::string_view& bytes) const;

    template int default_deserializer<int, std::enable_if_t<std::is_integral_v<int>>>::
                 operator()(const std::string_view& bytes) const;

    template unsigned int default_deserializer<unsigned int, std::enable_if_t<std::is_integral_v<unsigned int>>>::
                          operator()(const std::string_view& bytes) const;

    template short default_deserializer<short, std::enable_if_t<std::is_integral_v<short>>>::
                   operator()(const std::string_view& bytes) const;

    template unsigned short default_deserializer<unsigned short, std::enable_if_t<std::is_integral_v<unsigned short>>>::
                            operator()(const std::string_view& bytes) const;

    template long default_deserializer<long, std::enable_if_t<std::is_integral_v<long>>>::
                  operator()(const std::string_view& bytes) const;

    template unsigned long default_deserializer<unsigned long, std::enable_if_t<std::is_integral_v<unsigned long>>>::
                           operator()(const std::string_view& bytes) const;

    template long long default_deserializer<long long, std::enable_if_t<std::is_integral_v<long long>>>::
                       operator()(const std::string_view& bytes) const;

    template unsigned long long
    default_deserializer<unsigned long long, std::enable_if_t<std::is_integral_v<unsigned long long>>>::
    operator()(const std::string_view& bytes) const;
#endif


}
