/*! \file
    \brief A storage.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <memory>

#include <tetengo/trie/storage.h>

#include "tetengo_trie_storage.hpp"

struct tetengo_trie_trie;


tetengo_trie_storage* tetengo_trie_storage_createStorage(const tetengo_trie_trie* /*p_trie*/)
{
    auto p_instance = std::make_unique<tetengo_trie_storage>();
    return p_instance.release();
}

tetengo_trie_storage* tetengo_trie_storage_createMemoryStorage(const path_character_type* const /*path*/)
{
    return nullptr;
}

void tetengo_trie_storage_destroy(const tetengo_trie_storage* p_storage)
{
    const std::unique_ptr<const tetengo_trie_storage> p_instance{ p_storage };
}
