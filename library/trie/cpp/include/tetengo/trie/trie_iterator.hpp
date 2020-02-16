/*! \file
    \brief A trie iterator.

    Copyright (C) 2019-2020 kaoru
*/

#if !defined(TETENGO_TRIE_TRIEITERATOR_HPP)
#define TETENGO_TRIE_TRIEITERATOR_HPP

#include <any>
#include <iterator>
#include <utility>

#include <boost/iterator/iterator_facade.hpp>

#include <tetengo/trie/double_array_iterator.hpp>


namespace tetengo::trie
{
    class storage;


    /*!
        \brief An implementation of trie iterator.
    */
    class trie_iterator_impl
    {
    public:
        // types

        // constructors and destructor

        /*!
            \brief Creates an iterator.

            \param double_array_iterator_ A double array iterator.
            \param storage_               A storage.
        */
        trie_iterator_impl(double_array_iterator double_array_iterator_, storage& storage_);

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
        std::any& dereference() const;

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


    private:
        // variables

        double_array_iterator m_double_array_iterator;

        storage* m_p_storage;
    };


    /*!
        \brief A trie iterator.

        \tparam Value A value type.
    */
    template <typename Value>
    class trie_iterator :
    public boost::iterators::iterator_facade<trie_iterator<Value>, Value, std ::forward_iterator_tag>
    {
    public:
        // types

        //! The value type.
        using value_type = Value;


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
            return *std::any_cast<value_type>(&m_impl.dereference());
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
