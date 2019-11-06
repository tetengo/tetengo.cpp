/*! \file
    \brief A trie set.

    Copyright (C) 2019 kaoru
*/

#if !defined(TETENGO_TRIE_TRIESET_HPP)
#define TETENGO_TRIE_TRIESET_HPP

#include <algorithm>
#include <cstddef>
#include <cstdint>
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
        using key_serializer_type = KeySerializer;

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
        trie_set() : m_p_double_array{ std::make_unique<double_array>() }, m_values{}, m_stored_value_indexes{} {}

        /*!
            \brief Creates a trie set.

            \tparam InputIterator An input iterator type.

            \param first          A first element to insert.
            \param last           A last element to insert.
            \param key_serializer A key serializer.
        */
        template <typename InputIterator>
        trie_set(InputIterator first, InputIterator last, key_serializer_type key_serializer = key_serializer_type{}) :
        m_p_double_array{},
            m_values{},
            m_stored_value_indexes{},
            m_key_serializer{ std::move(key_serializer) }
        {
            make_double_array_and_values(
                first, last, m_key_serializer, m_p_double_array, m_values, m_stored_value_indexes);
        }

        /*!
            \brief Creates a trie set.

            \param initial_values Initial values.
            \param key_serializer A key serializer.
        */
        trie_set(
            std::initializer_list<value_type> initial_values,
            key_serializer_type               key_serializer = key_serializer_type{}) :
        m_p_double_array{},
            m_values{},
            m_stored_value_indexes{},
            m_key_serializer{ std::move(key_serializer) }
        {
            make_double_array_and_values(
                std::begin(initial_values),
                std::end(initial_values),
                m_key_serializer,
                m_p_double_array,
                m_values,
                m_stored_value_indexes);
        }


        // functions

        /*!
            \brief Returns the size.

            \return The size.
        */
        size_type size() const noexcept
        {
            return m_values.size();
        }

        /*!
            \brief Finds an element.

            \param key A key.

            \return A const iterator to an element.
        */
        const_iterator find(const key_type& key) const
        {
            m_p_double_array->find(m_key_serializer(key));
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
            const key_serializer_type&     key_serializer,
            std::unique_ptr<double_array>& p_double_array,
            std::vector<value_type>&       values,
            std::vector<std::int32_t>&     stored_value_indexes)
        {
            if (first == last)
            {
                return;
            }

            const std::vector<std::pair<std::string, int>> double_array_elements =
                make_double_array_elements(first, last, key_serializer);
            p_double_array = make_double_array(double_array_elements, stored_value_indexes);

            values = make_values(first, double_array_elements.size(), stored_value_indexes);
        }

        template <typename InputIterator>
        static std::vector<std::pair<std::string, int>> make_double_array_elements(
            const InputIterator        first,
            const InputIterator        last,
            const key_serializer_type& key_serializer)
        {
            std::vector<std::pair<std::string, int>> elements;
            elements.reserve(std::distance(first, last));

            auto index = 0;
            for (auto iter = first; iter != last; ++iter)
            {
                elements.emplace_back(key_serializer(*iter), index);
                ++index;
            }

            return elements;
        }

        static std::unique_ptr<double_array> make_double_array(
            const std::vector<std::pair<std::string, int>>& elements,
            std::vector<std::int32_t>&                      stored_value_indexes)
        {
            stored_value_indexes.resize(elements.size(), -1);
            std::int32_t                               storage_index = 0;
            const double_array::building_observer_type observer{
                [&stored_value_indexes, &storage_index](const std::pair<std::string, std::int32_t>& element) {
                    stored_value_indexes[element.second] = storage_index;
                    ++storage_index;
                },
                []() {}
            };

            return std::make_unique<double_array>(elements, observer);
        }

        template <typename InputIterator>
        static std::vector<value_type> make_values(
            const InputIterator        first,
            const std::size_t          element_count,
            std::vector<std::int32_t>& stored_value_indexes)
        {
            std::vector<value_type> values(
                static_cast<std::size_t>(*std::max_element(stored_value_indexes.begin(), stored_value_indexes.end())) +
                    1,
                *first);

            for (auto i = static_cast<std::size_t>(0); i < element_count; ++i)
            {
                const auto svi = stored_value_indexes[i];
                if (svi < 0)
                {
                    continue;
                }
                values[svi] = *std::next(first, i);
            }

            return values;
        }


        // variables

        std::unique_ptr<double_array> m_p_double_array;

        std::vector<value_type> m_values;

        std::vector<std::int32_t> m_stored_value_indexes;

        key_serializer_type m_key_serializer;
    };
}


#endif
