/*! \file
    \brief A trie iterator.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <iterator>
#include <memory>
#include <utility>

#include <boost/iterator/iterator_facade.hpp>

#include <tetengo/trie/trie.hpp>
#include <tetengo/trie/trieIterator.h>
#include <tetengo/trie/trie_iterator.hpp>

#include "tetengo_trie_trie.hpp"

struct tetengo_trie_trieIterator;


tetengo_trie_trieIterator* tetengo_trie_trieIterator_create(const tetengo_trie_trie* const p_trie)
{
    auto p_cpp_iterator_pair = std::make_unique<std::pair<trie_type::iterator, trie_type::iterator>>(
        std::begin(*p_trie->p_cpp_trie), std::end(*p_trie->p_cpp_trie));
    return reinterpret_cast<tetengo_trie_trieIterator*>(p_cpp_iterator_pair.release());
}

void tetengo_trie_trieIterator_destroy(const tetengo_trie_trieIterator* const p_iterator)
{
    std::unique_ptr<const std::pair<trie_type::iterator, trie_type::iterator>> p_iterator_pair{
        reinterpret_cast<const std::pair<trie_type::iterator, trie_type::iterator>*>(p_iterator)
    };
}

const void* tetengo_trie_trieIterator_get(const tetengo_trie_trieIterator* p_iterator)
{
    const auto* const p_cpp_iterator_pair =
        reinterpret_cast<const std::pair<trie_type::iterator, trie_type::iterator>*>(p_iterator);
    return *p_cpp_iterator_pair->first;
}

int tetengo_trie_trieIterator_hasNext(const tetengo_trie_trieIterator* p_iterator)
{
    const auto* const p_cpp_iterator_pair =
        reinterpret_cast<const std::pair<trie_type::iterator, trie_type::iterator>*>(p_iterator);
    return p_cpp_iterator_pair->first == p_cpp_iterator_pair->second ? 0 : 1;
}

void tetengo_trie_trieIterator_next(tetengo_trie_trieIterator* p_iterator)
{
    auto* const p_cpp_iterator_pair =
        reinterpret_cast<std::pair<trie_type::iterator, trie_type::iterator>*>(p_iterator);
    ++p_cpp_iterator_pair->first;
}
