/*! \file
    \brief A trie.

    Copyright (C) 2019 kaoru
*/

#if !defined(TETENGO_TRIE_TRIE_HPP)
#define TETENGO_TRIE_TRIE_HPP

#include <memory>

#include <boost/core/noncopyable.hpp>


namespace tetengo::trie
{
    /*!
        \brief An implementation of trie.
    */
    class trie_impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates an implementation of trie.
        */
        trie_impl();

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

        \tparam Key   A key type.
        \tparam Value A value type.
    */
    template <typename Key, typename Value>
    class trie : private boost::noncopyable
    {
    public:
        // types

        //! The key type.
        using key_type = Key;

        //! The value type.
        using value_type = Value;


        // constructors and destructor

        /*!
            \brief Creates a trie.
        */
        trie() : m_impl{} {}


    private:
        // variables

        const trie_impl m_impl;
    };


}


#endif
