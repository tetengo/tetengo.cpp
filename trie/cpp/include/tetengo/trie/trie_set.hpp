/*! \file
    \brief A trie set.

    Copyright (C) 2019 kaoru
*/

#if !defined(TETENGO_TRIE_TRIESET_HPP)
#define TETENGO_TRIE_TRIESET_HPP

#include <cstddef>
#include <initializer_list>

#include <boost/iterator/transform_iterator.hpp>

#include <tetengo/trie/default_key_serializer.hpp>
#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/memory_storage.hpp>


namespace tetengo::trie
{
    /*!
        \brief An enumerator.

        \tparam Key           A key type.
        \tparam Storage       A storage type.
        \tparam KeySerializer A key serializer type.
    */
    template <typename Key, typename Storage = memory_storage, typename KeySerializer = default_key_serializer<Key>>
    class trie_set
    {
    public:
        // types

        //! The key type.
        using key_type = Key;

        //! The value type.
        using value_type = Key;

        //! The storage type.
        using storage_type = Storage;

        //! The key serializer type.
        using key_serializer = KeySerializer;

        //! The pointer type.
        using pointer = value_type*;

        //! The const pointer type.
        using const_pointer = const value_type*;

        //! The reference type.
        using reference = value_type&;

        //! The const reference type.
        using const_reference = const value_type&;

        //! The size type.
        using size_type = std::size_t;

        //! The difference type.
        using difference_type = std::ptrdiff_t;

        //! The iterator type.
        using iterator = pointer; // TODO Create a class iterator.

        //! The const iterator type.
        using const_iterator = const_pointer; // TODO Create a class iterator.


        // constructors and destructor

        /*!
            \brief Creates a trie set.
        */
        trie_set() : m_double_array{} {}

        /*!
            \brief Creates a trie set.

            \tparam InputIterator An input iterator type.

            \param first A first element to insert.
            \param last  A last element to insert.
        */
        template <typename InputIterator>
        trie_set(InputIterator first, InputIterator last) :
        m_double_array{ boost::make_transform_iterator(first, to_double_array_iterator),
                        boost::make_transform_iterator(last, to_double_array_iterator) }
        {}

        /*!
            \brief Creates a trie set.

            \param initial_values Initial values.
        */
        trie_set(std::initializer_list<value_type> /*initial_values*/) : m_double_array{} {}


        // functions

        /*!
            \brief Returns the size.

            \return The size.
        */
        size_type size() const noexcept
        {
            return 42;
        }

        /*!
            \brief Finds an element.

            \param key A key.

            \return A const iterator to an element.
        */
        const_iterator find(const key_type& /*key*/) const
        {
            return const_iterator{};
        }


    private:
        // static functions

        static std::pair<std::string, std::int32_t> to_double_array_iterator(const std::string& element)
        {
            return std::make_pair(element, 0);
        }


        // variables

        double_array m_double_array;
    };


}


#endif
