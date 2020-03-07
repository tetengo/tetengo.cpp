/*! \file
    \brief A trie.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TRIE_TRIE_HPP_)
#define TETENGO_TRIE_TRIE_HPP_

#include <string_view>
#include <vector>

#include <stddef.h>

#include <tetengo/trie/trie.hpp>


using trie_type = tetengo::trie::trie<std::string_view, std::vector<char>>;

struct tetengo_trie_trie
{
    trie_type* p_cpp_trie;
    size_t     element_value_size;

    tetengo_trie_trie(trie_type* const p_cpp_trie, const size_t element_value_size) :
    p_cpp_trie{ p_cpp_trie },
        element_value_size{ element_value_size }
    {}
};


#endif
