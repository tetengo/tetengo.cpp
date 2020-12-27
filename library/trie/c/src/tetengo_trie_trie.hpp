/*! \file
    \brief A trie.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TRIE_TRIE_HPP_)
#define TETENGO_TRIE_TRIE_HPP_

#include <memory>
#include <string_view>
#include <utility>
#include <vector>

#include <stddef.h>

#include <tetengo/trie/storage.h>
#include <tetengo/trie/trie.hpp>

#include "tetengo_trie_storage.hpp" // IWYU pragma: keep


using cpp_trie_type = tetengo::trie::trie<std::string_view, std::vector<char>>;

struct tetengo_trie_trie_tag
{
    std::unique_ptr<cpp_trie_type> p_cpp_trie;

    size_t element_value_size;

    std::unique_ptr<tetengo_trie_storage_t> p_storage;

    tetengo_trie_trie_tag(std::unique_ptr<cpp_trie_type>&& p_cpp_trie, const size_t element_value_size) :
    p_cpp_trie{ std::move(p_cpp_trie) },
        element_value_size{ element_value_size },
        p_storage{}
    {}

    ~tetengo_trie_trie_tag()
    {
        tetengo_trie_storage_destroy(p_storage.release());
    }
};


#endif
