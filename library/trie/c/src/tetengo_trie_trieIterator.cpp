/*! \file
    \brief A trie iterator.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <iterator>
#include <memory>
#include <utility>
#include <vector>

#include <boost/iterator/iterator_facade.hpp>

#include <tetengo/trie/trie.hpp>
#include <tetengo/trie/trieIterator.h>
#include <tetengo/trie/trie_iterator.hpp>

#include "tetengo_trie_trie.hpp"


struct tetengo_trie_trieIterator
{
    std::unique_ptr<std::pair<cpp_trie_type::iterator, cpp_trie_type::iterator>> p_cpp_iterator_pair;

    tetengo_trie_trieIterator(
        std::unique_ptr<std::pair<cpp_trie_type::iterator, cpp_trie_type::iterator>>&& p_cpp_iterator_pair) :
    p_cpp_iterator_pair{ std::move(p_cpp_iterator_pair) }
    {}
};

tetengo_trie_trieIterator* tetengo_trie_trieIterator_create(const tetengo_trie_trie* const p_trie)
{
    auto p_cpp_iterator_pair = std::make_unique<std::pair<cpp_trie_type::iterator, cpp_trie_type::iterator>>(
        std::begin(*p_trie->p_cpp_trie), std::end(*p_trie->p_cpp_trie));
    auto p_instance = std::make_unique<tetengo_trie_trieIterator>(std::move(p_cpp_iterator_pair));
    return p_instance.release();
}

void tetengo_trie_trieIterator_destroy(const tetengo_trie_trieIterator* const p_iterator)
{
    const std::unique_ptr<const tetengo_trie_trieIterator> p_instance{ p_iterator };
}

const void* tetengo_trie_trieIterator_get(const tetengo_trie_trieIterator* p_iterator)
{
    return p_iterator->p_cpp_iterator_pair->first->data();
}

int tetengo_trie_trieIterator_hasNext(const tetengo_trie_trieIterator* p_iterator)
{
    return p_iterator->p_cpp_iterator_pair->first == p_iterator->p_cpp_iterator_pair->second ? 0 : 1;
}

void tetengo_trie_trieIterator_next(tetengo_trie_trieIterator* p_iterator)
{
    ++p_iterator->p_cpp_iterator_pair->first;
}
