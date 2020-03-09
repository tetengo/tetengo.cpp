/*! \file
    \brief A storage.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TRIE_STORAGE_HPP_)
#define TETENGO_TRIE_STORAGE_HPP_

#include <cassert>
#include <memory>
#include <utility>

#include <tetengo/trie/storage.hpp>


struct tetengo_trie_storage
{
private:
    const tetengo::trie::storage* p_cpp_storage_referred;

public:
    std::unique_ptr<tetengo::trie::storage> p_cpp_storage_owned;

    explicit tetengo_trie_storage(const tetengo::trie::storage* p_cpp_storage) :
    p_cpp_storage_referred{ p_cpp_storage },
        p_cpp_storage_owned{}
    {}

    explicit tetengo_trie_storage(std::unique_ptr<tetengo::trie::storage>&& p_cpp_storage) :
    p_cpp_storage_referred{},
        p_cpp_storage_owned{ std::move(p_cpp_storage) }
    {}

    const tetengo::trie::storage* p_cpp_storage() const
    {
        assert(!p_cpp_storage_referred || !p_cpp_storage_owned);
        return p_cpp_storage_referred ? p_cpp_storage_referred : p_cpp_storage_owned.get();
    }
};


#endif
