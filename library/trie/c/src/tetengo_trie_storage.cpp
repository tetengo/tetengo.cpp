/*! \file
    \brief A storage.

    Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/
*/

#include <any>
#include <climits>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <functional>
#include <limits>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include <boost/interprocess/exceptions.hpp>
#include <boost/interprocess/file_mapping.hpp>

#include <tetengo/trie/double_array.hpp>
#include <tetengo/trie/memory_storage.hpp>
#include <tetengo/trie/mmap_storage.hpp>
#include <tetengo/trie/shared_storage.hpp>
#include <tetengo/trie/storage.h>
#include <tetengo/trie/storage.hpp>
#include <tetengo/trie/trie.hpp>
#include <tetengo/trie/value_serializer.hpp>

#include "tetengo_trie_storage.hpp"
#include "tetengo_trie_trie.hpp"


unsigned char tetengo_trie_storage_vacantCheckValue()
{
    return tetengo::trie::double_array::vacant_check_value();
}

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

namespace
{
    std::unique_ptr<boost::interprocess::file_mapping> create_file_mapping(const path_character_type* const path)
    {
        try
        {
            return std::make_unique<boost::interprocess::file_mapping>(path, boost::interprocess::read_only);
        }
        catch (const boost::interprocess::interprocess_exception&)
        {
            return std::unique_ptr<boost::interprocess::file_mapping>{};
        }
    }

}

tetengo_trie_storage_t*
tetengo_trie_storage_createMmapStorage(const path_character_type* const path, const size_t content_offset)
{
    try
    {
        if (!path)
        {
            throw std::invalid_argument{ "path is NULL." };
        }

        auto                              p_file_mapping = create_file_mapping(path);
        const auto                        file_size = static_cast<std::size_t>(std::filesystem::file_size(path));
        tetengo::trie::value_deserializer deserializer{ [](const std::vector<char>& serialized) {
            return serialized;
        } };
        auto p_storage = std::make_unique<tetengo::trie::mmap_storage>(
            *p_file_mapping, content_offset, file_size, std::move(deserializer));
        auto p_instance = std::make_unique<tetengo_trie_storage_t>(std::move(p_file_mapping), std::move(p_storage));
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

bool tetengo_trie_storage_setBaseAt(
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

        return true;
    }
    catch (...)
    {
        return false;
    }
}

unsigned char tetengo_trie_storage_checkAt(const tetengo_trie_storage_t* const p_storage, const size_t base_check_index)
{
    try
    {
        if (!p_storage)
        {
            throw std::invalid_argument{ "p_storage is NULL." };
        }

        return p_storage->p_cpp_storage()->check_at(base_check_index);
    }
    catch (...)
    {
        return UCHAR_MAX;
    }
}

bool tetengo_trie_storage_setCheckAt(
    tetengo_trie_storage_t* const p_storage,
    const size_t                  base_check_index,
    const unsigned char           check)
{
    try
    {
        if (!p_storage)
        {
            throw std::invalid_argument{ "p_storage is NULL." };
        }

        p_storage->p_cpp_storage()->set_check_at(base_check_index, check);

        return true;
    }
    catch (...)
    {
        return false;
    }
}

size_t tetengo_trie_storage_valueCount(const tetengo_trie_storage_t* const p_storage)
{
    try
    {
        if (!p_storage)
        {
            throw std::invalid_argument{ "p_storage is NULL." };
        }

        return p_storage->p_cpp_storage()->value_count();
    }
    catch (...)
    {
        return static_cast<size_t>(-1);
    }
}

const void* tetengo_trie_storage_valueAt(const tetengo_trie_storage_t* const p_storage, const size_t value_index)
{
    try
    {
        if (!p_storage)
        {
            throw std::invalid_argument{ "p_storage is NULL." };
        }

        const auto* const cpp_value = p_storage->p_cpp_storage()->value_at(value_index);
        if (!cpp_value)
        {
            return NULL;
        }
        return std::any_cast<std::vector<char>>(cpp_value);
    }
    catch (...)
    {
        return NULL;
    }
}

bool tetengo_trie_storage_addValueAt(
    tetengo_trie_storage_t* const p_storage,
    const size_t                  value_index,
    const void* const             p_value,
    const size_t                  value_size)
{
    try
    {
        if (!p_storage)
        {
            throw std::invalid_argument{ "p_storage is NULL." };
        }
        if (!p_value)
        {
            throw std::invalid_argument{ "p_value is NULL." };
        }

        auto value_bytes = std::make_any<std::vector<char>>(
            static_cast<const char*>(p_value), static_cast<const char*>(p_value) + value_size);
        p_storage->p_cpp_storage()->add_value_at(value_index, std::move(value_bytes));

        return true;
    }
    catch (...)
    {
        return false;
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

bool tetengo_trie_storage_serialize(
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

        return true;
    }
    catch (...)
    {
        return false;
    }
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
