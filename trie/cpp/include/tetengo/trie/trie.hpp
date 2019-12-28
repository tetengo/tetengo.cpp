/*! \file
    \brief A trie.

    Copyright (C) 2019 kaoru
*/

#if !defined(TETENGO_TRIE_TRIE_HPP)
#define TETENGO_TRIE_TRIE_HPP

#include <algorithm>
#include <any>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <boost/core/noncopyable.hpp>

#include <tetengo/trie/default_serializer.hpp>


namespace tetengo::trie
{
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

        //! The vaue type.
        using value_type = std::pair<key_type, mapped_type>;

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


        // constructors and destructor

        /*!
            \brief Creates an implementation of trie.
        */
        trie_impl();

        /*!
            \brief Creates a trie.

            \param elements              Initial elements.
            \param building_observer_set A building observer set.
        */
        explicit trie_impl(std::vector<value_type> elements, const building_observer_set_type& building_observer_set);

        /*!
            \brief Destroys an implementation of trie.
        */
        ~trie_impl();


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

        //! The value type.
        using value_type = std::pair<key_type, mapped_type>;

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


        // constructors and destructor

        /*!
            \brief Creates a trie.
        */
        trie() : m_impl{} {}

        /*!
            \brief Creates a trie.

            \param elements              Initial elements.
            \param key_serializer        A key serializer.
            \param building_observer_set A building observer set.
        */
        explicit trie(
            std::initializer_list<value_type> elements,
            const key_serializer_type&        key_serializer = default_serializer<key_type>{},
            const building_observer_set_type& building_observer_set = null_building_observer_set()) :
        m_impl{ serialize_key(elements, key_serializer), building_observer_set }
        {}


    private:
        // static functions

        static std::vector<trie_impl::value_type>
        serialize_key(std::initializer_list<value_type> elements, const key_serializer_type& key_serializer)
        {
            std::vector<trie_impl::value_type> serialized{};
            serialized.reserve(elements.size());
            std::transform(
                std::begin(elements),
                std::end(elements),
                std::back_inserter(serialized),
                [&key_serializer](auto& value) {
                    return trie_impl::value_type{ key_serializer(value.first), std::move(value.second) };
                });
            return serialized;
        }


        // variables

        const trie_impl m_impl;
    };


}


#endif
