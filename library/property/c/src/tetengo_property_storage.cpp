/*! \file
    \brief A storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>

#include <stddef.h>
#include <stdint.h> // IWYU pragma: keep

#include <tetengo/property/file_storage.hpp>
#include <tetengo/property/memory_storage.hpp>
#include <tetengo/property/storage.h>
#include <tetengo/property/storage.hpp>


struct tetengo_property_storage_tag
{
    std::unique_ptr<tetengo::property::storage> p_cpp_storage;

    explicit tetengo_property_storage_tag(std::unique_ptr<tetengo::property::storage>&& p_cpp_storage) :
    p_cpp_storage{ std::move(p_cpp_storage) }
    {}
};

struct tetengo_property_storageLoader_tag
{
    std::unique_ptr<tetengo::property::storage_loader> p_cpp_storage_loader;

    explicit tetengo_property_storageLoader_tag(
        std::unique_ptr<tetengo::property::storage_loader>&& p_cpp_storage_loader) :
    p_cpp_storage_loader{ std::move(p_cpp_storage_loader) }
    {}
};


void tetengo_property_storage_destroy(const tetengo_property_storage_t* const p_storage)
{
    try
    {
        const std::unique_ptr<const tetengo_property_storage_t> p_instance{ p_storage };
    }
    catch (...)
    {}
}

int tetengo_property_storage_getBool(
    const tetengo_property_storage_t* const p_storage,
    const char* const                       key,
    int* const                              p_value)
{
    try
    {
        if (!p_storage)
        {
            throw std::invalid_argument{ "p_storage is NULL." };
        }
        if (!key)
        {
            throw std::invalid_argument{ "key is NULL." };
        }
        if (!p_value)
        {
            throw std::invalid_argument{ "p_value is NULL." };
        }

        const auto o_cpp_value = p_storage->p_cpp_storage->get_bool(key);
        if (o_cpp_value)
        {
            *p_value = *o_cpp_value ? 1 : 0;
        }
        return static_cast<bool>(o_cpp_value);
    }
    catch (...)
    {
        return 0;
    }
}

void tetengo_property_storage_setBool(
    tetengo_property_storage_t* const p_storage,
    const char* const                 key,
    const int                         value)
{
    try
    {
        if (!p_storage)
        {
            throw std::invalid_argument{ "p_storage is NULL." };
        }
        if (!key)
        {
            throw std::invalid_argument{ "key is NULL." };
        }

        p_storage->p_cpp_storage->set_bool(key, value);
    }
    catch (...)
    {}
}

int tetengo_property_storage_getUint32(
    const tetengo_property_storage_t* const p_storage,
    const char* const                       key,
    uint32_t* const                         p_value)
{
    try
    {
        if (!p_storage)
        {
            throw std::invalid_argument{ "p_storage is NULL." };
        }
        if (!key)
        {
            throw std::invalid_argument{ "key is NULL." };
        }
        if (!p_value)
        {
            throw std::invalid_argument{ "p_value is NULL." };
        }

        const auto o_cpp_value = p_storage->p_cpp_storage->get_uint32(key);
        if (o_cpp_value)
        {
            *p_value = *o_cpp_value;
        }
        return static_cast<bool>(o_cpp_value);
    }
    catch (...)
    {
        return 0;
    }
}

void tetengo_property_storage_setUint32(
    tetengo_property_storage_t* const p_storage,
    const char* const                 key,
    const uint32_t                    value)
{
    try
    {
        if (!p_storage)
        {
            throw std::invalid_argument{ "p_storage is NULL." };
        }
        if (!key)
        {
            throw std::invalid_argument{ "key is NULL." };
        }

        p_storage->p_cpp_storage->set_uint32(key, value);
    }
    catch (...)
    {}
}

size_t tetengo_property_storage_getString(
    const tetengo_property_storage_t* const p_storage,
    const char* const                       key,
    char* const                             p_value,
    const size_t                            value_capacity)
{
    try
    {
        if (!p_storage)
        {
            throw std::invalid_argument{ "p_storage is NULL." };
        }
        if (!key)
        {
            throw std::invalid_argument{ "key is NULL." };
        }

        const auto o_cpp_value = p_storage->p_cpp_storage->get_string(key);
        if (o_cpp_value)
        {
            if (p_value && value_capacity > 0)
            {
                const auto length_to_copy = std::min(o_cpp_value->length(), value_capacity - 1);
                std::copy_n(std::begin(*o_cpp_value), length_to_copy, p_value);
                p_value[length_to_copy] = '\0';
            }
            return o_cpp_value->length();
        }
        else
        {
            return 0;
        }
    }
    catch (...)
    {
        return 0;
    }
}

void tetengo_property_storage_setString(
    tetengo_property_storage_t* const p_storage,
    const char* const                 key,
    const char* const                 value)
{
    try
    {
        if (!p_storage)
        {
            throw std::invalid_argument{ "p_storage is NULL." };
        }
        if (!key)
        {
            throw std::invalid_argument{ "key is NULL." };
        }

        p_storage->p_cpp_storage->set_string(key, value);
    }
    catch (...)
    {}
}

void tetengo_property_storage_save(const tetengo_property_storage_t* const p_storage)
{
    try
    {
        if (!p_storage)
        {
            throw std::invalid_argument{ "p_storage is NULL." };
        }

        p_storage->p_cpp_storage->save();
    }
    catch (...)
    {}
}

tetengo_property_storageLoader_t* tetengo_property_storageLoader_createMemoryStorageLoader()
{
    try
    {
        auto p_cpp_storage_loader = std::make_unique<tetengo::property::memory_storage_loader>();

        auto p_instance = std::make_unique<tetengo_property_storageLoader_t>(std::move(p_cpp_storage_loader));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

tetengo_property_storageLoader_t* tetengo_property_storageLoader_createFileStorageLoader()
{
    try
    {
        auto p_cpp_storage_loader = std::make_unique<tetengo::property::file_storage_loader>();

        auto p_instance = std::make_unique<tetengo_property_storageLoader_t>(std::move(p_cpp_storage_loader));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

void tetengo_property_storageLoader_destroy(const tetengo_property_storageLoader_t* const p_storage_loader)
{
    try
    {
        const std::unique_ptr<const tetengo_property_storageLoader_t> p_instance{ p_storage_loader };
    }
    catch (...)
    {}
}

tetengo_property_storage_t* tetengo_property_storageLoader_load(
    const tetengo_property_storageLoader_t* const p_storage_loader,
    const char* const                             path)
{
    try
    {
        if (!p_storage_loader)
        {
            throw std::invalid_argument{ "p_storage_loader is NULL." };
        }
        if (!path)
        {
            throw std::invalid_argument{ "path is NULL." };
        }

        auto p_cpp_storage = p_storage_loader->p_cpp_storage_loader->load(path);

        auto p_instance = std::make_unique<tetengo_property_storage_t>(std::move(p_cpp_storage));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}
