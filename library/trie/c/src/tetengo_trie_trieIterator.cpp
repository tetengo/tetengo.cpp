/*! \file
    \brief A trie iterator.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <iterator>
#include <memory>
#include <string_view>
#include <utility>

#include <tetengo/trie/trie.hpp>
#include <tetengo/trie/trieIterator.h>

struct tetengo_trie_trie;
struct tetengo_trie_trieIterator;


namespace
{
    using trie_type = tetengo::trie::trie<std::string_view, const void*>;
}


tetengo_trie_trieIterator* tetengo_trie_trieIterator_create(const tetengo_trie_trie* const p_trie)
{
    const auto* const p_cpp_trie = reinterpret_cast<const trie_type*>(p_trie);
    auto              p_iterator_pair = std::make_unique<std::pair<trie_type::iterator, trie_type::iterator>>(
        std::begin(*p_cpp_trie), std::end(*p_cpp_trie));
    return reinterpret_cast<tetengo_trie_trieIterator*>(p_iterator_pair.release());
}

void tetengo_trie_trieIterator_destroy(tetengo_trie_trieIterator* const p_trie_iterator)
{
    std::unique_ptr<std::pair<trie_type::iterator, trie_type::iterator>> p_iterator_pair{
        reinterpret_cast<std::pair<trie_type::iterator, trie_type::iterator>*>(p_trie_iterator)
    };
}
