/*! \file
    \brief A default key serializer.

    Copyright (C) 2019 kaoru
*/

#if !defined(TETENGO_TRIE_DEFAULTKEYSERIALIZER_HPP)
#define TETENGO_TRIE_DEFAULTKEYSERIALIZER_HPP

#include <string>


namespace tetengo::trie
{
    /*!
        \brief A default key serializer.

        \tparam Key A key type.
    */
    template <typename Key>
    class default_key_serializer;


#if !defined(DOCUMENTATION)
    template <>
    class default_key_serializer<std::string>
    {
    public:
        // functions

        const std::string& operator()(const std::string& key) const
        {
            return key;
        }
    };


    template <typename Char>
    class default_key_serializer<std::basic_string<Char>>
    {
    public:
        // functions

        std::string operator()(const std::basic_string<Char>& key) const;
    };


#endif
}


#endif
