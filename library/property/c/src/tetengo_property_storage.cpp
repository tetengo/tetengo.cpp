/*! \file
    \brief A storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <memory>
#include <stdexcept>
#include <utility>

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
