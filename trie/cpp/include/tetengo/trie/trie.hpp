/*! \file
    \brief A trie.

    Copyright (C) 2019 kaoru
*/

#if !defined(TETENGO_TRIE_TRIE_HPP)
#define TETENGO_TRIE_TRIE_HPP

#include <algorithm>
#include <any>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <string>
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


        // constructors and destructor

        /*!
            \brief Creates an implementation of trie.
        */
        trie_impl();

        /*!
            \brief Creates a trie.

            \param elements Initial elements.
        */
        explicit trie_impl(std::vector<value_type> elements);

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


        // constructors and destructor

        /*!
            \brief Creates a trie.
        */
        trie() : m_impl{} {}

        /*!
            \brief Creates a trie.

            \param elements       Initial elements.
            \param key_serializer A key serializer.
        */
        explicit trie(
            std::initializer_list<value_type> elements,
            const key_serializer_type&        key_serializer = default_serializer<key_type>{}) :
        m_impl{ serialize_key(elements, key_serializer) }
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
