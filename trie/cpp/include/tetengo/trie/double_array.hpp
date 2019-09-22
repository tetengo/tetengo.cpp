/*! \file
    \brief A double array.

    Copyright (C) 2019 kaoru
*/

#if !defined(TETENGO_TRIE_DOUBLEARRAY_HPP)
#define TETENGO_TRIE_DOUBLEARRAY_HPP

#include <cstdint>


namespace tetengo::trie
{
    /*!
        \brief A double array.
    */
    class double_array
    {
    public:
        // static functions

        /*!
            \brief Make a base-check value.

            \param base  A base value.
            \param check A check value.
        */
        static std::uint32_t make_base_check(std::int32_t base, std::int8_t check);

        /*!
            \brief Returns the base value from a base-check value.

            \param base_check A base-check value.

            \return The base value.
        */
        static std::int32_t base_of(std::uint32_t base_check);

        /*!
            \brief Returns the check value from a base-check value.

            \param base_check A base-check value.

            \return The check value.
        */
        static std::int8_t check_of(std::uint32_t base_check);


        // constructors and destructor

        /*!
            \brief Creates a double array.
        */
        double_array();
    };


}


#endif
