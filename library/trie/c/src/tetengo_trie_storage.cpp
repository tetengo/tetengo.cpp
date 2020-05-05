/*! \file
    \brief A storage.

    Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <cstddef>
#include <fstream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include <tetengo/trie/memory_storage.hpp>
#include <tetengo/trie/shared_storage.hpp>
#include <tetengo/trie/storage.h>
#include <tetengo/trie/storage.hpp>
#include <tetengo/trie/trie.h>
#include <tetengo/trie/trie.hpp>

#include "tetengo_trie_storage.hpp"
#include "tetengo_trie_trie.hpp" // IWYU pragma: keep


tetengo_trie_storage_t* tetengo_trie_storage_createStorage(const tetengo_trie_trie_t* const p_trie)
{
    try
    {
        if (!p_trie)
        {
            throw std::invalid_argument{ "p_trie is NULL." };
        }

        auto p_instance =
            std::make_unique<tetengo_trie_storage_t>(&p_trie->p_cpp_trie->get_storage(), p_trie->element_value_size);
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

        std::ifstream stream{ path, std::ios_base::binary };
        std::size_t   element_value_size = std::numeric_limits<std::size_t>::max();
        auto          p_storage = std::make_unique<tetengo::trie::memory_storage>(
            stream, [&element_value_size](const std::vector<char>& serialized) {
                if (element_value_size == std::numeric_limits<std::size_t>::max())
                {
                    element_value_size = serialized.size();
                }
                return serialized;
            });
        auto p_instance = std::make_unique<tetengo_trie_storage_t>(std::move(p_storage), element_value_size);
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

        std::ifstream stream{ path, std::ios_base::binary };
        std::size_t   element_value_size = std::numeric_limits<std::size_t>::max();
        auto          p_storage = std::make_unique<tetengo::trie::shared_storage>(
            stream, [&element_value_size](const std::vector<char>& serialized) {
                if (element_value_size == std::numeric_limits<std::size_t>::max())
                {
                    element_value_size = serialized.size();
                }
                return serialized;
            });
        auto p_instance = std::make_unique<tetengo_trie_storage_t>(std::move(p_storage), element_value_size);
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

size_t tetengo_trie_storage_size(const tetengo_trie_storage_t* const p_storage)
{
    try
    {
        if (!p_storage)
        {
            throw std::invalid_argument{ "p_storage is NULL." };
        }

        return p_storage->p_cpp_storage()->size();
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
    const path_character_type* const    path)
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

        std::ofstream stream{ path, std::ios_base::binary };
        p_storage->p_cpp_storage()->serialize(
            stream, [](const std::any& value) { return *std::any_cast<std::vector<char>>(&value); });
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

        auto p_instance = std::make_unique<tetengo_trie_storage_t>(std::move(p_clone), p_storage->element_value_size);
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}
