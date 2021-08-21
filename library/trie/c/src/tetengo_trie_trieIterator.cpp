/*! \file
    \brief A trie iterator.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <iterator>
#include <memory>
#include <stdexcept>
#include <utility>

#include <tetengo/trie/storage.h>
#include <tetengo/trie/trie.h>
#include <tetengo/trie/trie.hpp>
#include <tetengo/trie/trieIterator.h>
#include <tetengo/trie/trie_iterator.hpp>

#include "tetengo_trie_trie.hpp"


struct tetengo_trie_trieIterator_tag
{
    std::unique_ptr<std::pair<cpp_trie_type::iterator, cpp_trie_type::iterator>> p_cpp_iterator_pair;

    explicit tetengo_trie_trieIterator_tag(
        std::unique_ptr<std::pair<cpp_trie_type::iterator, cpp_trie_type::iterator>>&& p_cpp_iterator_pair) :
    p_cpp_iterator_pair{ std::move(p_cpp_iterator_pair) }
    {}
};

tetengo_trie_trieIterator_t* tetengo_trie_trieIterator_create(const tetengo_trie_trie_t* const p_trie)
{
    try
    {
        if (!p_trie)
        {
            throw std::invalid_argument{ "p_trie is NULL." };
        }

        auto p_cpp_iterator_pair = std::make_unique<std::pair<cpp_trie_type::iterator, cpp_trie_type::iterator>>(
            std::begin(*p_trie->p_cpp_trie), std::end(*p_trie->p_cpp_trie));
        auto p_instance = std::make_unique<tetengo_trie_trieIterator_t>(std::move(p_cpp_iterator_pair));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

void tetengo_trie_trieIterator_destroy(const tetengo_trie_trieIterator_t* const p_iterator)
{
    try
    {
        const std::unique_ptr<const tetengo_trie_trieIterator_t> p_instance{ p_iterator };
    }
    catch (...)
    {}
}

const void* tetengo_trie_trieIterator_get(const tetengo_trie_trieIterator_t* p_iterator)
{
    try
    {
        if (!p_iterator)
        {
            throw std::invalid_argument{ "p_iterator is NULL." };
        }

        return std::data(*p_iterator->p_cpp_iterator_pair->first);
    }
    catch (...)
    {
        return nullptr;
    }
}

int tetengo_trie_trieIterator_hasNext(const tetengo_trie_trieIterator_t* p_iterator)
{
    try
    {
        if (!p_iterator)
        {
            throw std::invalid_argument{ "p_iterator is NULL." };
        }

        return p_iterator->p_cpp_iterator_pair->first == p_iterator->p_cpp_iterator_pair->second ? 0 : 1;
    }
    catch (...)
    {
        return 0;
    }
}

void tetengo_trie_trieIterator_next(tetengo_trie_trieIterator_t* p_iterator)
{
    try
    {
        if (!p_iterator)
        {
            throw std::invalid_argument{ "p_iterator is NULL." };
        }

        ++p_iterator->p_cpp_iterator_pair->first;
    }
    catch (...)
    {}
}
