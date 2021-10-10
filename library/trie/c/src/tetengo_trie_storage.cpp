/*! \file
    \brief A storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <climits>
#include <cstddef>
#include <fstream>
#include <functional>
#include <limits>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include <tetengo/trie/memory_storage.hpp>
#include <tetengo/trie/mmap_storage.hpp>
#include <tetengo/trie/shared_storage.hpp>
#include <tetengo/trie/storage.h>
#include <tetengo/trie/storage.hpp>
#include <tetengo/trie/trie.hpp>
#include <tetengo/trie/value_serializer.hpp>

#include "tetengo_trie_storage.hpp"
#include "tetengo_trie_trie.hpp"


tetengo_trie_storage_t* tetengo_trie_storage_createStorage(const tetengo_trie_trie_t* const p_trie)
{
    try
    {
        if (!p_trie)
        {
            throw std::invalid_argument{ "p_trie is NULL." };
        }

        auto p_instance = std::make_unique<tetengo_trie_storage_t>(&p_trie->p_cpp_trie->get_storage());
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

tetengo_trie_storage_t* tetengo_trie_storage_createMemoryStorage(const path_character_type* const path)
{
    try
    {
        if (!path)
        {
            throw std::invalid_argument{ "path is NULL." };
        }

        std::ifstream                           stream{ path, std::ios_base::binary };
        const tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>& serialized) {
            return serialized;
        } };
        auto p_storage = std::make_unique<tetengo::trie::memory_storage>(stream, deserializer);
        auto p_instance = std::make_unique<tetengo_trie_storage_t>(std::move(p_storage));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

tetengo_trie_storage_t* tetengo_trie_storage_createSharedStorage(const path_character_type* const path)
{
    try
    {
        if (!path)
        {
            throw std::invalid_argument{ "path is NULL." };
        }

        std::ifstream                           stream{ path, std::ios_base::binary };
        const tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>& serialized) {
            return serialized;
        } };
        auto p_storage = std::make_unique<tetengo::trie::shared_storage>(stream, deserializer);
        auto p_instance = std::make_unique<tetengo_trie_storage_t>(std::move(p_storage));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

tetengo_trie_storage_t*
tetengo_trie_storage_createMmapStorage(const path_character_type* const path, const size_t offset)
{
    try
    {
        if (!path)
        {
            throw std::invalid_argument{ "path is NULL." };
        }

        auto p_storage = std::make_unique<tetengo::trie::mmap_storage>(path, offset);
        auto p_instance = std::make_unique<tetengo_trie_storage_t>(std::move(p_storage));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

void tetengo_trie_storage_destroy(const tetengo_trie_storage_t* const p_storage)
{
    try
    {
        const std::unique_ptr<const tetengo_trie_storage_t> p_instance{ p_storage };
    }
    catch (...)
    {}
}

size_t tetengo_trie_storage_baseCheckSize(const tetengo_trie_storage_t* const p_storage)
{
    try
    {
        if (!p_storage)
        {
            throw std::invalid_argument{ "p_storage is NULL." };
        }

        return p_storage->p_cpp_storage()->base_check_size();
    }
    catch (...)
    {
        return static_cast<size_t>(-1);
    }
}

int tetengo_trie_storage_baseAt(const tetengo_trie_storage_t* const p_storage, const size_t base_check_index)
{
    try
    {
        if (!p_storage)
        {
            throw std::invalid_argument{ "p_storage is NULL." };
        }

        return p_storage->p_cpp_storage()->base_at(base_check_index);
    }
    catch (...)
    {
        return INT_MAX;
    }
}

int tetengo_trie_storage_setBaseAt(
    tetengo_trie_storage_t* const p_storage,
    const size_t                  base_check_index,
    const int                     base)
{
    try
    {
        if (!p_storage)
        {
            throw std::invalid_argument{ "p_storage is NULL." };
        }

        p_storage->p_cpp_storage()->set_base_at(base_check_index, base);

        return 1;
    }
    catch (...)
    {
        return 0;
    }
}

size_t tetengo_trie_storage_valueSize(const tetengo_trie_storage_t* const p_storage)
{
    try
    {
        if (!p_storage)
        {
            throw std::invalid_argument{ "p_storage is NULL." };
        }

        return p_storage->p_cpp_storage()->value_size();
    }
    catch (...)
    {
        return static_cast<size_t>(-1);
    }
}

double tetengo_trie_storage_fillingRate(const tetengo_trie_storage_t* const p_storage)
{
    try
    {
        if (!p_storage)
        {
            throw std::invalid_argument{ "p_storage is NULL." };
        }

        return p_storage->p_cpp_storage()->filling_rate();
    }
    catch (...)
    {
        return std::numeric_limits<double>::signaling_NaN();
    }
}

void tetengo_trie_storage_serialize(
    const tetengo_trie_storage_t* const p_storage,
    const path_character_type* const    path,
    const size_t                        fixed_value_size)
{
    try
    {
        if (!p_storage)
        {
            throw std::invalid_argument{ "p_storage is NULL." };
        }
        if (!path)
        {
            throw std::invalid_argument{ "path is NULL." };
        }

        std::ofstream                         stream{ path, std::ios_base::binary };
        const tetengo::trie::value_serializer serializer{
            [](const std::any& value) { return *std::any_cast<std::vector<char>>(&value); }, fixed_value_size
        };
        p_storage->p_cpp_storage()->serialize(stream, serializer);
    }
    catch (...)
    {}
}

tetengo_trie_storage_t* tetengo_trie_storage_clone(const tetengo_trie_storage_t* const p_storage)
{
    try
    {
        if (!p_storage)
        {
            throw std::invalid_argument{ "p_storage is NULL." };
        }
        auto p_clone = p_storage->p_cpp_storage()->clone();

        auto p_instance = std::make_unique<tetengo_trie_storage_t>(std::move(p_clone));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}
