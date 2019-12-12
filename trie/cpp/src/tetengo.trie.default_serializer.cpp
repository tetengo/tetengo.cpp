/*! \file
    \brief A default serializer.

    Copyright (C) 2019 kaoru
*/

#include <cstddef>
#include <string>
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

    template std::string
    default_serializer<std::basic_string<wchar_t>>::operator()(const std::basic_string<wchar_t>& object) const;

    template std::string
    default_serializer<std::basic_string<char16_t>>::operator()(const std::basic_string<char16_t>& object) const;

    template std::string
    default_serializer<std::basic_string<char32_t>>::operator()(const std::basic_string<char32_t>& object) const;


    template <typename Integer>
    std::string default_serializer<Integer, std::enable_if_t<std::is_integral<Integer>::value>>::operator()(
        const Integer object) const
    {
        return to_bytes(object);
    }

    template std::string
    default_serializer<bool, std::enable_if_t<std::is_integral<bool>::value>>::operator()(bool object) const;

    template std::string
    default_serializer<char, std::enable_if_t<std::is_integral<char>::value>>::operator()(char object) const;

    template std::string
    default_serializer<signed char, std::enable_if_t<std::is_integral<signed char>::value>>::operator()(
        signed char object) const;

    template std::string
    default_serializer<unsigned char, std::enable_if_t<std::is_integral<unsigned char>::value>>::operator()(
        unsigned char object) const;

    template std::string
    default_serializer<wchar_t, std::enable_if_t<std::is_integral<wchar_t>::value>>::operator()(wchar_t object) const;

    template std::string default_serializer<char16_t, std::enable_if_t<std::is_integral<char16_t>::value>>::operator()(
        char16_t object) const;

    template std::string default_serializer<char32_t, std::enable_if_t<std::is_integral<char32_t>::value>>::operator()(
        char32_t object) const;

    template std::string
    default_serializer<int, std::enable_if_t<std::is_integral<int>::value>>::operator()(int object) const;

    template std::string
    default_serializer<unsigned int, std::enable_if_t<std::is_integral<unsigned int>::value>>::operator()(
        unsigned int object) const;

    template std::string
    default_serializer<short, std::enable_if_t<std::is_integral<short>::value>>::operator()(short object) const;

    template std::string
    default_serializer<unsigned short, std::enable_if_t<std::is_integral<unsigned short>::value>>::operator()(
        unsigned short object) const;

    template std::string
    default_serializer<long, std::enable_if_t<std::is_integral<long>::value>>::operator()(long object) const;

    template std::string
    default_serializer<unsigned long, std::enable_if_t<std::is_integral<unsigned long>::value>>::operator()(
        unsigned long object) const;

    template std::string
    default_serializer<long long, std::enable_if_t<std::is_integral<long long>::value>>::operator()(
        long long object) const;

    template std::string
    default_serializer<unsigned long long, std::enable_if_t<std::is_integral<unsigned long long>::value>>::operator()(
        unsigned long long object) const;
#endif


}
