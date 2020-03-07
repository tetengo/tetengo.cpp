/*! \file
    \brief A storage.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <tetengo/trie/storage.h>


struct tetengo_trie_storage
{};

tetengo_trie_storage* tetengo_trie_storage_createMemoryStorage(const path_character_type* const /*path*/)
{
    return nullptr;
}

void tetengo_trie_storage_destroy(const tetengo_trie_storage* /*p_storage*/) {}
