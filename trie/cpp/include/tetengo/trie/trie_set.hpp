/*! \file
    \brief A trie set.

    Copyright (C) 2019 kaoru
*/

#if !defined(TETENGO_TRIE_TRIESET_HPP)
#define TETENGO_TRIE_TRIESET_HPP

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <tetengo/trie/default_key_serializer.hpp>
#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/memory_storage.hpp>
#include <tetengo/trie/trie_set_iterator.hpp>


namespace tetengo::trie
{
    class enumerator;


    /*!
        \brief A trie set.

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

        //! The const iterator type.
        using const_iterator = trie_set_iterator<value_type>;


        // constructors and destructor

        /*!
            \brief Creates a trie set.
        */
        trie_set() : m_p_double_array{ std::make_unique<double_array>() }, m_values{} {}

        /*!
            \brief Creates a trie set.

            \tparam InputIterator An input iterator type.

            \param first A first element to insert.
            \param last  A last element to insert.
        */
        template <typename InputIterator>
        trie_set(InputIterator first, InputIterator last) : m_p_double_array{}, m_values{}
        {
            make_double_array_and_values(first, last, m_p_double_array, m_values);
        }

        /*!
            \brief Creates a trie set.

            \param initial_values Initial values.
        */
        trie_set(std::initializer_list<value_type> initial_values) : m_p_double_array{}, m_values{}
        {
            make_double_array_and_values(
                std::begin(initial_values), std::end(initial_values), m_p_double_array, m_values);
        }


        // functions

        /*!
            \brief Returns the size.

            \return The size.
        */
        size_type size() const noexcept
        {
            return std::count_if(
                std::begin(m_p_double_array->get_storage().values()),
                std::end(m_p_double_array->get_storage().values()),
                [](const auto& e) { return (e & 0x000000FF) == 0x00; });
        }

        /*!
            \brief Finds an element.

            \param key A key.

            \return A const iterator to an element.
        */
        const_iterator find(const key_type& /*key*/) const
        {
            return begin();
        }

        /*!
            \brief Returns the first iterator.

            \return The first iterator.
        */
        const_iterator begin() const
        {
            return const_iterator{ nullptr,
                                   std::make_unique<enumerator>(m_p_double_array->get_enumerator()),
                                   &m_values };
        }

        /*!
            \brief Returns the last iterator.

            \return The last iterator.
        */
        const_iterator end() const
        {
            return const_iterator{ nullptr, nullptr, nullptr };
        }


    private:
        // static functions

        template <typename InputIterator>
        static void make_double_array_and_values(
            InputIterator                  first,
            InputIterator                  last,
            std::unique_ptr<double_array>& p_double_array,
            std::vector<value_type>&       values)
        {
            std::vector<std::pair<std::string, int>> elements;
            elements.reserve(std::distance(first, last));
            std::transform(
                first, last, std::back_inserter(elements), [](const auto& e) { return std::make_pair(e, 0); });
            p_double_array = std::make_unique<double_array>(elements);
            values.assign(first, last);
        }


        // variables

        std::unique_ptr<double_array> m_p_double_array;

        std::vector<value_type> m_values;
    };


}


#endif
