/*! \file
    \brief A trie iterator.

    Copyright (C) 2019-2023 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TRIE_TRIEITERATOR_HPP)
#define TETENGO_TRIE_TRIEITERATOR_HPP

#include <any>
#include <iterator>
#include <utility>

#include <boost/stl_interfaces/iterator_interface.hpp>

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
            \brief Dereferences the iterator.

            \return The dereferenced value.
        */
        [[nodiscard]] const std::any& operator*() const;

        /*!
            \brief Dereferences the iterator.

            \return The dereferenced value.
        */
        [[nodiscard]] std::any& operator*();

        /*!
            \brief Returns true when one iterator is equal to another.

            \param one   One iterator.
            \param another Another iterator.

            \retval true  When one is equal to another.
            \retval false Otherwise.
        */
        friend bool operator==(const trie_iterator_impl& one, const trie_iterator_impl& another);

        /*!
            \brief Increments the iterator.

            \return This iterator.
        */
        trie_iterator_impl& operator++();


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
    public boost::stl_interfaces::iterator_interface<trie_iterator<Value>, std ::forward_iterator_tag, Value>
    {
    public:
        // types

        //! The value type.
        using value_type = Value;


        // constructors and destructor

        /*!
            \brief Creates an iterator.

            \param impl An implementation of trie iterator.
        */
        trie_iterator(trie_iterator_impl&& impl) : m_impl{ std::move(impl) } {}


        // functions

        /*!
            \brief Dereferences the iterator.

            \return The dereferenced value.
        */
        [[nodiscard]] const value_type& operator*() const
        {
            return *std::any_cast<value_type>(&*m_impl);
        }

        /*!
            \brief Dereferences the iterator.

            \return The dereferenced value.
        */
        [[nodiscard]] value_type& operator*()
        {
            return *std::any_cast<value_type>(&*m_impl);
        }

        /*!
            \brief Returns the pointer to the value.

            \return The pointer to the value.
        */
        [[nodiscard]] const value_type* operator->() const
        {
            return std::any_cast<value_type>(&*m_impl);
        }

        /*!
            \brief Returns the pointer to the value.

            \return The pointer to the value.
        */
        [[nodiscard]] value_type* operator->()
        {
            return std::any_cast<value_type>(&*m_impl);
        }

        /*!
            \brief Returns true when one iterator is equal to another.

            \param one   One iterator.
            \param another Another iterator.

            \retval true  When one is equal to another.
            \retval false Otherwise.
        */
        friend bool operator==(const trie_iterator& one, const trie_iterator& another)
        {
            return one.m_impl == another.m_impl;
        }

        /*!
            \brief Increments the iterator.

            \return This iterator.
        */
        trie_iterator& operator++()
        {
            ++m_impl;
            return *this;
        }

        /*!
            \brief Postincrements the iterator.

            \return The iterator before the incrementation.
        */
        trie_iterator operator++(int)
        {
            trie_iterator original{ *this };
            ++(*this);
            return original;
        }


    private:
        // variables

        trie_iterator_impl m_impl;
    };


}


#endif
