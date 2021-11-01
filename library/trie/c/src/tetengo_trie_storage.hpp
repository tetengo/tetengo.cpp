/*! \file
    \brief A storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_TRIE_STORAGE_HPP_)
#define TETENGO_TRIE_STORAGE_HPP_

#include <cassert>
#include <memory>
#include <utility>

#include <boost/interprocess/file_mapping.hpp>

#include <tetengo/trie/storage.hpp>


struct tetengo_trie_storage_tag
{
private:
    std::unique_ptr<boost::interprocess::file_mapping> p_cpp_file_mapping;

    const tetengo::trie::storage* p_cpp_storage_referred;

public:
    std::unique_ptr<tetengo::trie::storage> p_cpp_storage_owned;

    explicit tetengo_trie_storage_tag(const tetengo::trie::storage* const p_cpp_storage) :
    p_cpp_file_mapping{},
    p_cpp_storage_referred{ p_cpp_storage },
    p_cpp_storage_owned{}
    {}

    explicit tetengo_trie_storage_tag(std::unique_ptr<tetengo::trie::storage>&& p_cpp_storage) :
    p_cpp_file_mapping{},
    p_cpp_storage_referred{},
    p_cpp_storage_owned{ std::move(p_cpp_storage) }
    {}

    tetengo_trie_storage_tag(
        std::unique_ptr<boost::interprocess::file_mapping>&& p_cpp_file_mapping,
        std::unique_ptr<tetengo::trie::storage>&&            p_cpp_storage) :
    p_cpp_file_mapping{ std::move(p_cpp_file_mapping) },
    p_cpp_storage_referred{},
    p_cpp_storage_owned{ std::move(p_cpp_storage) }
    {}

    const tetengo::trie::storage* p_cpp_storage() const
    {
        assert(!p_cpp_storage_referred || !p_cpp_storage_owned);
        return p_cpp_storage_referred ? p_cpp_storage_referred : p_cpp_storage_owned.get();
    }

    tetengo::trie::storage* p_cpp_storage()
    {
        assert(!p_cpp_storage_referred);
        return p_cpp_storage_owned.get();
    }
};


#endif
