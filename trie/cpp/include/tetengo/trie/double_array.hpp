/*! \file
    \brief A double array.

    Copyright (C) 2019 kaoru
*/

#if !defined(TETENGO_TRIE_DOUBLEARRAY_HPP)
#define TETENGO_TRIE_DOUBLEARRAY_HPP

#include <algorithm>
#include <cstdint>
#include <iterator>
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

            \param element_pointers Pointers to initial elements.
        */
        explicit double_array(std::vector<const std::pair<std::string, std::int32_t>*> element_pointers);

        /*!
            \brief Creates a double array.

            \tparam InputIterator An input iterator type.

            \param first An iterator to the first element.
            \param last  An iterator to the last element.
        */
        template <typename InputIterator>
        double_array(InputIterator first, InputIterator last) :
        double_array{ [first, last]() {
            std::vector<const std::pair<std::string, std::int32_t>*> element_pointers;
            std::transform(first, last, std::back_inserter(element_pointers), [](const auto& e) { return &e; });
            return element_pointers;
        }() }
        {}

        /*!
            \brief Creates a double array.

            \param elements Initial elements.
        */
        explicit double_array(const std::vector<std::pair<std::string, std::int32_t>>& elements);


        // functions

        /*!
            \brief Returns the base check array.

            \return The base check array.
        */
        const std::vector<std::uint32_t>& base_check_array() const;


    private:
        // variables

        std::vector<std::uint32_t> m_base_check_array;
    };


}


#endif
