/*! \file
    \brief A trie set iterator.

    Copyright (C) 2019 kaoru
*/

#if !defined(TETENGO_TRIE_TRIESETITERATOR_HPP)
#define TETENGO_TRIE_TRIESETITERATOR_HPP

#include <stdexcept>

#include <boost/iterator/iterator_facade.hpp>


namespace boost::iterators
{
    struct forward_traversal_tag;
}


namespace tetengo::trie
{
    class double_array;


    /*!
        \brief A trie set iterator.

        \taparam Value A value type.
    */
    template <typename Value>
    class trie_set_iterator :
    public boost::iterator_facade<trie_set_iterator<Value>, Value, boost::forward_traversal_tag>
    {
    public:
        // types

        //! The value type.
        using value_type = Value;


        // constructors

        /*!
            \brief Creates a trie set iterator.

            \param p_first_element A pointer to the first element.
                                   Or nullptr for an iterator traversing all the elements.
            \param p_double_array  A pointer to the double array. Or nullptr for the last iterator.

            \throw std::invalid_argument When p_first_element is not nullptr but p_double_array is nullptr.
        */
        trie_set_iterator(const value_type* p_first_element, const double_array* p_double_array) :
        m_p_first_element{ p_first_element },
            m_p_double_array{ p_double_array }
        {
            if (m_p_first_element && !m_p_double_array)
            {
                throw std::invalid_argument{ "Null double array." };
            }
        }


    private:
        // variables

        const value_type* m_p_first_element;

        const double_array* m_p_double_array;
    };


}


#endif
