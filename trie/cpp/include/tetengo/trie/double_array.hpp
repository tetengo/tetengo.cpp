/*! \file
    \brief A double array.

    Copyright (C) 2019 kaoru
*/

#if !defined(TETENGO_TRIE_DOUBLEARRAY_HPP)
#define TETENGO_TRIE_DOUBLEARRAY_HPP

#include <cstdint>
#include <initializer_list>
#include <string>
#include <utility>
#include <vector>


namespace tetengo::trie
{
    /*!
        \brief A double array.
    */
    class double_array
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a double array.
        */
        double_array();

        /*!
            \brief Creates a double array.

            \param elements Initial elements.
        */
        explicit double_array(std::initializer_list<std::pair<std::string, std::int32_t>> elements);


        // functions

        /*!
            \brief Returns the base check array.

            \return The base check array.
        */
        const std::vector<std::uint32_t> base_check_array() const;


    private:
        // variables

        std::vector<std::uint32_t> m_base_check_array;
    };


}


#endif
