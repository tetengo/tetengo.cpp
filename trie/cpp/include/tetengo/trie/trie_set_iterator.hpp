/*! \file
    \brief A trie set iterator.

    Copyright (C) 2019 kaoru
*/

#if !defined(TETENGO_TRIE_TRIESETITERATOR_HPP)
#define TETENGO_TRIE_TRIESETITERATOR_HPP

#include <cassert>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include <boost/iterator/iterator_facade.hpp>

#include <tetengo/trie/enumerator.hpp>


namespace boost::iterators
{
    struct forward_traversal_tag;
}


namespace tetengo::trie
{
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
            \param p_enumerator    A unique pointer to the double array enumerator. Or nullptr for the last iterator.
            \param p_values        A pointer to the values. Or nullptr for the last iterator.

            \throw std::invalid_argument When p_first_element is not nullptr but p_enumerator or p_values is nullptr.
        */
        trie_set_iterator(
            const value_type*              p_first_element,
            std::unique_ptr<enumerator>    p_enumerator,
            const std::vector<value_type>* p_values) :
        m_p_first_element{ p_first_element },
            m_p_enumerator{ std::move(p_enumerator) },
            m_p_values{ p_values }
        {
            if (m_p_first_element && (!m_p_enumerator || p_values))
            {
                throw std::invalid_argument{ "Null double array." };
            }
            assert(!m_p_enumerator == !m_p_values);
        }


        // functions

        /*!
            \brief Dereferences this iterator.

            \return The value pointed to by this iterator.
        */
        value_type& dereference() const
        {
            if (m_p_first_element)
            {
                if (m_p_enumerator)
                {
                    const auto o_first_in_double_array = m_p_enumerator->next();
                    assert(
                        o_first_in_double_array &&
                        (*m_p_values)[o_first_in_double_array->second] == *m_p_first_element);
                }
                auto& first_element = const_cast<value_type&>(*m_p_first_element);
                m_p_first_element = nullptr;
                return first_element;
            }

            const auto o_element_in_double_array = m_p_enumerator->next();
            if (o_element_in_double_array)
            {
                return const_cast<value_type&>((*m_p_values)[o_element_in_double_array->second]);
            }
            else
            {
                throw std::logic_error{ "" };
            }
        }


    private:
        // variables

        mutable const value_type* m_p_first_element;

        std::unique_ptr<enumerator> m_p_enumerator;

        const std::vector<value_type>* m_p_values;
    };


}


#endif
