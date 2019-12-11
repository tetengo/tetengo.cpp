/*! \file
    \brief A default serializer.

    Copyright (C) 2019 kaoru
*/

#if !defined(TETENGO_TRIE_DEFAULTSERIALIZER_HPP)
#define TETENGO_TRIE_DEFAULTSERIALIZER_HPP

#include <string>


namespace tetengo::trie
{
    /*!
        \brief A default serializer.

        \tparam Key A key type.
    */
    template <typename Key>
    class default_serializer;


#if !defined(DOCUMENTATION)
    template <>
    class default_serializer<std::string>
    {
    public:
        // functions

        const std::string& operator()(const std::string& key) const
        {
            return key;
        }
    };


    template <typename Char>
    class default_serializer<std::basic_string<Char>>
    {
    public:
        // functions

        std::string operator()(const std::basic_string<Char>& key) const;
    };


#endif
}


#endif
