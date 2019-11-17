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
        \brief A trie.
    */
    class trie : private boost::noncopyable
    {
    public:
        // constructors and destructor

        /*!
            \brief Creates a trie.
        */
        trie();

        /*!
            \brief Destroys the trie.
        */
        ~trie();


    private:
        // types

        class impl;


        // variables

        const std::unique_ptr<impl> m_p_impl;
    };


}


#endif
