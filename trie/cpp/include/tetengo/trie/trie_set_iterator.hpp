/*! \file
    \brief A trie set iterator.

    Copyright (C) 2019 kaoru
*/

#if !defined(TETENGO_TRIE_TRIESETITERATOR_HPP)
#define TETENGO_TRIE_TRIESETITERATOR_HPP

#include <boost/iterator/iterator_facade.hpp>


namespace tetengo::trie
{
    /*!
        \brief A trie set iterator.

        \taparam Value A value type.
    */
    template <typename Value>
    class trie_set_iterator :
    public boost::iterator_facade<trie_set_iterator<Value>, Value, boost::forward_traversal_tag>
    {
    public:
        // types

        //! The value type.
        using value_type = Value;
    };


}


#endif
