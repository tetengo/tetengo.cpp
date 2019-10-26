/*! \file
    \brief A trie set.

    Copyright (C) 2019 kaoru
*/

#if !defined(TETENGO_TRIE_TRIESET_HPP)
#define TETENGO_TRIE_TRIESET_HPP

#include <cstddef>


namespace tetengo::trie
{
    /*!
        \brief An enumerator.

        \tparam Key     A key type.
        \tparam Storage A storage type.
    */
    template <typename Key, typename Storage>
    class trie_set
    {
    public:
        // types

        using key_type = Key;

        using value_type = Key;

        using storage_type = Storage;

        using pointer = value_type*;

        using const_pointer = const value_type*;

        using reference = value_type&;

        using const_reference = const value_type&;

        using size_type = std::size_t;

        using difference_type = std::ptrdiff_t;

        using iterator = pointer; // TODO Create a class iterator.

        using const_iterator = const_pointer; // TODO Create a class iterator.
    };


}


#endif
