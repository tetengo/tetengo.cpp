/*! \file
    \brief A storage.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TRIE_STORAGE_HPP_)
#define TETENGO_TRIE_STORAGE_HPP_

#include <cassert>
#include <memory>

#include <tetengo/trie/storage.hpp>


struct tetengo_trie_storage
{
private:
    std::unique_ptr<tetengo::trie::storage> p_cpp_storage_created;

    const tetengo::trie::storage* p_cpp_storage_referred;

public:
    explicit tetengo_trie_storage(const tetengo::trie::storage* p_cpp_storage) :
    p_cpp_storage_created{},
        p_cpp_storage_referred{ p_cpp_storage }
    {}

    const tetengo::trie::storage* p_cpp_storage() const
    {
        assert(!p_cpp_storage_created || !p_cpp_storage_referred);
        return p_cpp_storage_created ? p_cpp_storage_created.get() : p_cpp_storage_referred;
    }
};


#endif
