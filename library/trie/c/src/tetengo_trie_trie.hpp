/*! \file
    \brief A trie.

    Copyright (C) 2019-2026 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TRIE_TRIE_HPP_)
#define TETENGO_TRIE_TRIE_HPP_

#include <memory>
#include <string_view>
#include <utility>
#include <vector>

#include <boost/interprocess/file_mapping.hpp>

#include <tetengo/trie/storage.h>
#include <tetengo/trie/trie.hpp>

#include "tetengo_trie_storage.hpp"


using cpp_trie_type = tetengo::trie::trie<std::string_view, std::vector<char>>;

struct tetengo_trie_trie_tag
{
    std::unique_ptr<cpp_trie_type> p_cpp_trie;

    std::unique_ptr<boost::interprocess::file_mapping> p_cpp_file_mapping;

    std::unique_ptr<tetengo_trie_storage_t> p_storage;

    tetengo_trie_trie_tag(std::unique_ptr<cpp_trie_type>&& p_cpp_trie) :
    p_cpp_trie{ std::move(p_cpp_trie) },
    p_cpp_file_mapping{},
    p_storage{}
    {}

    tetengo_trie_trie_tag(
        std::unique_ptr<cpp_trie_type>&&                   p_cpp_trie,
        std::unique_ptr<boost::interprocess::file_mapping> p_cpp_file_mapping) :
    p_cpp_trie{ std::move(p_cpp_trie) },
    p_cpp_file_mapping{ std::move(p_cpp_file_mapping) },
    p_storage{}
    {}

    ~tetengo_trie_trie_tag()
    {
        tetengo_trie_storage_destroy(p_storage.release());
    }
};


#endif
