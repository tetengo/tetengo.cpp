/*! \file
    \brief A default key serializer.

    Copyright (C) 2019 kaoru
*/

#include <cstddef>
#include <string>

#include <tetengo/trie/default_key_serializer.hpp>


namespace tetengo::trie
{
#if !defined(DOCUMENTATION)
    const std::string& default_key_serializer<std::string>::operator()(const std::string& key) const
    {
        return key;
    }

    template <typename Char>
    std::string default_key_serializer<std::basic_string<Char>>::operator()(const std::basic_string<Char>& key) const
    {
        std::string serialized;
        serialized.reserve(key.length() * sizeof(Char) / sizeof(char));

        for (const auto c: key)
        {
            for (auto i = static_cast<std::size_t>(0); i < sizeof(Char); ++i)
            {
                const auto byte = static_cast<char>(static_cast<unsigned char>(c >> (sizeof(Char) - i - 1) * 8) & 0xFF);
                serialized.push_back(byte);
            }
        }

        return serialized;
    }

    template std::string default_key_serializer<std::basic_string<wchar_t>>::
                         operator()(const std::basic_string<wchar_t>& key) const;

    template std::string default_key_serializer<std::basic_string<char16_t>>::
                         operator()(const std::basic_string<char16_t>& key) const;

    template std::string default_key_serializer<std::basic_string<char32_t>>::
                         operator()(const std::basic_string<char32_t>& key) const;
#endif


}
