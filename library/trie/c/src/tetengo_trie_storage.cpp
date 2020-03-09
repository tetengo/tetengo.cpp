/*! \file
    \brief A storage.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <fstream>
#include <memory>
#include <utility>
#include <vector>

#include <stddef.h>

#include <tetengo/trie/memory_storage.hpp>
#include <tetengo/trie/storage.h>
#include <tetengo/trie/storage.hpp>
#include <tetengo/trie/trie.hpp>

#include "tetengo_trie_storage.hpp"
#include "tetengo_trie_trie.hpp"


tetengo_trie_storage* tetengo_trie_storage_createStorage(const tetengo_trie_trie* const p_trie)
{
    auto p_instance = std::make_unique<tetengo_trie_storage>(&p_trie->p_cpp_trie->get_storage());
    return p_instance.release();
}

tetengo_trie_storage* tetengo_trie_storage_createMemoryStorage(const path_character_type* const path)
{
    std::ifstream stream{ path, std::ios_base::binary };
    auto          p_storage = std::make_unique<tetengo::trie::memory_storage>(
        stream, [](const std::vector<char>& serialized) { return serialized; });
    auto p_instance = std::make_unique<tetengo_trie_storage>(std::move(p_storage));
    return p_instance.release();
}

void tetengo_trie_storage_destroy(const tetengo_trie_storage* const p_storage)
{
    const std::unique_ptr<const tetengo_trie_storage> p_instance{ p_storage };
}

size_t tetengo_trie_storage_size(const tetengo_trie_storage* const p_storage)
{
    return p_storage->p_cpp_storage()->size();
}

double tetengo_trie_storage_fillingRate(const tetengo_trie_storage* const p_storage)
{
    return p_storage->p_cpp_storage()->filling_rate();
}

void tetengo_trie_storage_serialize(const tetengo_trie_storage* const p_storage, const path_character_type* const path)
{
    std::ofstream stream{ path, std::ios_base::binary };
    p_storage->p_cpp_storage()->serialize(
        stream, [](const std::any& value) { return *std::any_cast<std::vector<char>>(&value); });
}
