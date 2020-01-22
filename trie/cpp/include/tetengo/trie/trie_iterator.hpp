/*! \file
    \brief A trie iterator.

    Copyright (C) 2019 kaoru
*/

#if !defined(TETENGO_TRIE_TRIEITERATOR_HPP)
#define TETENGO_TRIE_TRIEITERATOR_HPP

#include <cstdint>
#include <iterator>
#include <string>
#include <utility>

#include <boost/iterator/iterator_facade.hpp>


namespace tetengo::trie
{
    /*!
        \brief An implementation of trie iterator.
    */
    class trie_iterator_impl
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates an iterator.

            It points to the last of the double array.
        */
        trie_iterator_impl();


        // functions

        /*!
            \brief Dereferences this iterator.

            \return The dereferenced value.
        */
        std::pair<std::string, std::int32_t>& dereference() const;

        /*!
            \brief Returns true when this iterator is equal to another.

            \param another Another iterator.

            \retval true  When this iterator is equal to another.
        */
        bool equal(const trie_iterator_impl& another) const;

        /*!
            \brief Increments this iterator.
        */
        void increment();
    };


    /*!
        \brief A trie iterator.

        \tparam Key    A key type.
        \tparam Mapped A mapped type.
    */
    template <typename Key, typename Mapped>
    class trie_iterator :
    public boost::iterators::
        iterator_facade<trie_iterator<Key, Mapped>, std::pair<Key, Mapped>, std ::forward_iterator_tag>
    {
    public:
        // types

        //! The key type.
        using key_type = Key;

        //! The mapped type.
        using mapped_type = Mapped;

        //! The vaue type.
        using value_type = std::pair<key_type, mapped_type>;


        // friends

        //! Allows boost::iterator_facade to access the private members.
        friend class boost::iterators::iterator_core_access;


        // constructors and destructor

        /*!
            \brief Creates an iterator.

            \param impl An implementation of trie iterator.
        */
        trie_iterator(trie_iterator_impl&& impl) : m_impl{ std::move(impl) } {}


    private:
        // variables

        trie_iterator_impl m_impl;


        // functions

        value_type& dereference() const
        {
            return m_impl.dereference();
        }

        bool equal(const trie_iterator& another) const
        {
            return m_impl.equal(another.m_impl);
        }

        void increment()
        {
            m_impl.increment();
        }
    };


}


#endif
