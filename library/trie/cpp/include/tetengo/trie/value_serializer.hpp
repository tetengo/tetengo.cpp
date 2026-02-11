/*! \file
    \brief A value serializer.

    Copyright (C) 2019-2026 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TRIE_VALUESERIALIZER_HPP)
#define TETENGO_TRIE_VALUESERIALIZER_HPP

#include <any>
#include <cstddef>
#include <functional>
#include <vector>


namespace tetengo::trie
{
    /*!
        \brief A value serializer.
    */
    class value_serializer
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a value serializer.

            \param serialize        A serializing function.
            \param fixed_value_size The value size if it is fixed. Or 0 if the size is variable.
        */
        value_serializer(std::function<std::vector<char>(const std::any&)> serialize, std::size_t fixed_value_size);


        // functions

        /*!
            \brief Serializes a value.

            \param value A value.

            \return A serialized value.
        */
        std::vector<char> operator()(const std::any& value) const;

        /*!
            \brief Returns the fixed value size.

            \retval >0 The value size if it is fixed.
            \return 0  If the value size is variable.
        */
        std::size_t fixed_value_size() const;


    private:
        // variables

        std::function<std::vector<char>(const std::any&)> m_serialize;

        std::size_t m_fixed_value_size;
    };


    /*!
        \brief A value deserializer.
    */
    class value_deserializer
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a value deserializer.

            \param deserialize A deserializing function.
        */
        explicit value_deserializer(std::function<std::any(const std::vector<char>&)> deserialize);


        // functions

        /*!
            \brief Deserializes a value.

            \param serialized A serialized value.

            \return A value.
        */
        std::any operator()(const std::vector<char>& serialized) const;


    private:
        // variables

        std::function<std::any(const std::vector<char>&)> m_deserialize;
    };


}


#endif
