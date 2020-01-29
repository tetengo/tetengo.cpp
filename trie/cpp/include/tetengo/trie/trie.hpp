/*! \file
    \brief A trie.

    Copyright (C) 2019 kaoru
*/

#if !defined(TETENGO_TRIE_TRIE_HPP)
#define TETENGO_TRIE_TRIE_HPP

#include <algorithm>
#include <any>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <boost/core/noncopyable.hpp>

#include <tetengo/trie/default_serializer.hpp>
#include <tetengo/trie/trie_iterator.hpp>


namespace tetengo::trie
{
    class storage;


    /*!
        \brief An implementation of trie.
    */
    class trie_impl : private boost::noncopyable
    {
    public:
        // types

        //! The key type.
        using key_type = std::string;

        //! The mapped type.
        using mapped_type = std::any;

        //! The building observer set type.
        struct building_observer_set_type
        {
            /*!
                \brief Called when a key is adding.

                \param serialized_key A serialized key.
            */
            std::function<void(const std::string_view& serialized_key)> adding;

            /*!
                \brief Called when the building is done.
            */
            std::function<void()> done;
        };


        // static functions

        /*!
            \brief Returns the null building observer set.

            \return The null bulding observer set.
        */
        static const building_observer_set_type& null_building_observer_set();

        /*!
            \brief Returns the default double array density factor.

            \return The default double array density factor.
        */
        static std::size_t default_double_array_density_factor();


        // constructors and destructor

        /*!
            \brief Creates an implementation of trie.
        */
        trie_impl();

        /*!
            \brief Creates a trie.

            \param elements                    Initial elements.
            \param building_observer_set       A building observer set.
            \param double_array_density_factor A double array density factor.
         */
        trie_impl(
            std::vector<std::pair<key_type, mapped_type>> elements,
            const building_observer_set_type&             building_observer_set,
            std::size_t                                   double_array_density_factor);

        /*!
            \brief Creates a trie.

            \param p_storage A unique pointer to a storage.
        */
        explicit trie_impl(std::unique_ptr<storage>&& p_storage);

        /*!
            \brief Destroys an implementation of trie.
        */
        ~trie_impl();


        // functions

        /*!
            \brief Finds the mapped object correspoinding the given key.

            \param key A key.

            \return The mapped object. Or std::nullpot when the trie does not have the given key.
        */
        std::optional<mapped_type> find(const key_type& key) const;

        /*!
            \brief Returns the first iterator.

            \return The first iterator.
        */
        trie_iterator_impl begin() const;

        /*!
            \brief Returns the last iterator.

            \return The last iterator.
        */
        trie_iterator_impl end() const;

        /*!
            \brief Returns the storage.

            \return The storage.
        */
        const storage& get_storage() const;


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;
    };


    /*!
        \brief A trie.

        \tparam Key           A key type.
        \tparam Mapped        A mapped type.
        \tparam KeySerializer A key serializer type.
    */
    template <typename Key, typename Mapped, typename KeySerializer = default_serializer<Key>>
    class trie : private boost::noncopyable
    {
    public:
        // types

        //! The key type.
        using key_type = Key;

        //! The mapped type.
        using mapped_type = Mapped;

        //! The key serializer_type.
        using key_serializer_type = KeySerializer;

        //! The iterator type.
        using iterator = trie_iterator<mapped_type>;

        //! The building observer set type.
        using building_observer_set_type = trie_impl::building_observer_set_type;


        // static functions

        /*!
            \brief Returns the null building observer set.

            \return The null bulding observer set.
        */
        static const building_observer_set_type& null_building_observer_set()
        {
            return trie_impl::null_building_observer_set();
        }

        /*!
            \brief Returns the default double array density factor.

            \return The default double array density factor.
        */
        static std::size_t default_double_array_density_factor()
        {
            return trie_impl::default_double_array_density_factor();
        }


        // constructors and destructor

        /*!
            \brief Creates a trie.

            \param key_serializer A key serializer.
        */
        explicit trie(const key_serializer_type& key_serializer = default_serializer<key_type>{}) :
        m_impl{},
            m_key_serializer{ key_serializer }
        {}

        /*!
            \brief Creates a trie.

            \param elements                    Initial elements.
            \param key_serializer              A key serializer.
            \param building_observer_set       A building observer set.
            \param double_array_density_factor A double array density factor.
        */
        explicit trie(
            std::initializer_list<std::pair<key_type, mapped_type>> elements,
            const key_serializer_type&                              key_serializer = default_serializer<key_type>{},
            const building_observer_set_type& building_observer_set = null_building_observer_set(),
            std::size_t                       double_array_density_factor = default_double_array_density_factor()) :
        m_impl{ serialize_key(elements, key_serializer), building_observer_set, double_array_density_factor },
            m_key_serializer{ key_serializer }
        {}

        /*!
            \brief Creates a trie.

            \param p_storage      A unique pointer to a storage.
            \param key_serializer A key serializer.
        */
        explicit trie(
            std::unique_ptr<storage>&& p_storage,
            const key_serializer_type& key_serializer = default_serializer<key_type>{}) :
        m_impl{ std::move(p_storage) },
            m_key_serializer{ key_serializer }
        {}


        // functions

        /*!
            \brief Finds the mapped object correspoinding the given key.

            \param key A key.

            \return The mapped object. Or std::nullpot when the trie does not have the given key.
        */
        std::optional<mapped_type> find(const key_type& key) const
        {
            const auto o_found = m_impl.find(m_key_serializer(key));
            if (o_found)
            {
                return std::make_optional(std::any_cast<mapped_type>(*o_found));
            }
            else
            {
                return std::nullopt;
            }
        }

        /*!
            \brief Returns the first iterator.

            \return The first iterator.
        */
        iterator begin() const
        {
            return iterator{ std::move(m_impl.begin()) };
        }

        /*!
            \brief Returns the last iterator.

            \return The last iterator.
        */
        iterator end() const
        {
            return iterator{ std::move(m_impl.end()) };
        }

        /*!
            \brief Returns the storage.

            \return The storage.
        */
        const storage& get_storage() const
        {
            return m_impl.get_storage();
        }


    private:
        // static functions

        static std::vector<std::pair<trie_impl::key_type, trie_impl::mapped_type>> serialize_key(
            std::initializer_list<std::pair<key_type, mapped_type>> elements,
            const key_serializer_type&                              key_serializer)
        {
            std::vector<std::pair<trie_impl::key_type, trie_impl::mapped_type>> serialized{};
            serialized.reserve(elements.size());
            std::transform(
                std::begin(elements),
                std::end(elements),
                std::back_inserter(serialized),
                [&key_serializer](auto& value) {
                    return std::pair<trie_impl::key_type, trie_impl::mapped_type>{ key_serializer(value.first),
                                                                                   std::move(value.second) };
                });
            return serialized;
        }


        // variables

        const trie_impl m_impl;

        const key_serializer_type m_key_serializer;
    };


}


#endif
