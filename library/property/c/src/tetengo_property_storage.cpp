/*! \file
    \brief A storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <memory>
#include <utility>

#include <tetengo/property/memory_storage.hpp>
#include <tetengo/property/storage.h>


struct tetengo_property_storageLoader_tag
{
    std::unique_ptr<tetengo::property::memory_storage_loader> p_cpp_storage_loader;

    explicit tetengo_property_storageLoader_tag(
        std::unique_ptr<tetengo::property::memory_storage_loader>&& p_cpp_storage_loader) :
    p_cpp_storage_loader{ std::move(p_cpp_storage_loader) }
    {}
};


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
