/*! \file
    \brief A storage.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TRIE_STORAGE_HPP_)
#define TETENGO_TRIE_STORAGE_HPP_

namespace tetengo::trie
{
    class storage;
}


struct tetengo_trie_storage
{
    const tetengo::trie::storage* p_cpp_storage;

    explicit tetengo_trie_storage(const tetengo::trie::storage* p_cpp_storage) : p_cpp_storage{ p_cpp_storage } {}
};


#endif
