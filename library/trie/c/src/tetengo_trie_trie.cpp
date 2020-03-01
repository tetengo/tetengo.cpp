/*! \file
    \brief A trie.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <stddef.h>

#include <tetengo/trie/trie.h>
#include <tetengo/trie/trie.hpp>

struct tetengo_trie_trie;


void tetengo_trie_trie_nullAddingObserver(const char*) {}

void tetengo_trie_trie_nullDoneObserver() {}

size_t tetengo_trie_trie_defaultDoubleArrayDensityFactor()
{
    return tetengo::trie::trie_impl::default_double_array_density_factor();
}

tetengo_trie_trie* tetengo_trie_trie_create()
{
    return nullptr;
}

void tetengo_trie_trie_destroy(tetengo_trie_trie* const /* p_trie */) {}
