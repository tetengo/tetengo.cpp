/*! \file
    \brief A default serializer.

    Copyright (C) 2019-2020 kaoru
*/

#if !defined(TETENGO_TRIE_DEFAULTSERIALIZER_HPP)
#define TETENGO_TRIE_DEFAULTSERIALIZER_HPP

#include <string>
#include <string_view>
#include <type_traits>
#include <vector>


namespace tetengo::trie
{
    /*!
        \brief A default serializer.

        Binary bytes except std::string are serialized as following:

        <table>
          <tr><th>original byte</th><th>serialized byte</th></tr>
          <tr><td>0x00     </td><td>0xFE       (0b11111110)            </td></tr>
          <tr><td>0x01-0xFC</td><td>0x01-0xFC  (0b00000001-0b11111100) </td></tr>
          <tr><td>0xFD     </td><td>0xFD, 0xFD (0b11111101, 0b11111101)</td></tr>
          <tr><td>0xFE     </td><td>0xFD, 0xFE (0b11111101, 0b11111110)</td></tr>
          <tr><td>0xFF     </td><td>0xFF       (0b11111111)            </td></tr>
        </table>

        \tparam Object An object type.
    */
    template <typename Object, typename = void>
    class default_serializer;


#if !defined(DOCUMENTATION)
    template <>
    class default_serializer<std::string_view>
    {
    public:
        // functions

        const std::string_view& operator()(const std::string_view& object) const;
    };

    template <>
    class default_serializer<std::string>
    {
    public:
        // functions

        const std::string& operator()(const std::string& object) const;
    };

    template <typename Char>
    class default_serializer<std::basic_string_view<Char>>
    {
    public:
        // functions

        std::vector<char> operator()(const std::basic_string_view<Char>& object) const;
    };

    template <typename Char>
    class default_serializer<std::basic_string<Char>>
    {
    public:
        // functions

        std::vector<char> operator()(const std::basic_string<Char>& object) const;
    };

    template <typename Integer>
    class default_serializer<Integer, std::enable_if_t<std::is_integral_v<Integer>>>
    {
    public:
        // functions

        std::vector<char> operator()(Integer object) const;
    };

#endif


    /*!
        \brief A default deserializer.

        Binary bytes except std::string are deserialized as following:

        <table>
          <tr><th>original byte</th><th>serialized byte</th></tr>
          <tr><td>0x00     </td><td>0xFE       (0b11111110)            </td></tr>
          <tr><td>0x01-0xFC</td><td>0x01-0xFC  (0b00000001-0b11111100) </td></tr>
          <tr><td>0xFD     </td><td>0xFD, 0xFD (0b11111101, 0b11111101)</td></tr>
          <tr><td>0xFE     </td><td>0xFD, 0xFE (0b11111101, 0b11111110)</td></tr>
          <tr><td>0xFF     </td><td>0xFF       (0b11111111)            </td></tr>
        </table>

        \tparam Object An object type.
    */
    template <typename Object, typename = void>
    class default_deserializer;


#if !defined(DOCUMENTATION)
    template <>
    class default_deserializer<std::string>
    {
    public:
        // functions

        const std::string& operator()(const std::string& bytes) const;
    };

    template <typename Char>
    class default_deserializer<std::basic_string<Char>>
    {
    public:
        // functions

        std::basic_string<Char> operator()(const std::vector<char>& bytes) const;
    };

    template <typename Integer>
    class default_deserializer<Integer, std::enable_if_t<std::is_integral_v<Integer>>>
    {
    public:
        // functions

        Integer operator()(const std::vector<char>& bytes) const;
    };

#endif
}


#endif
