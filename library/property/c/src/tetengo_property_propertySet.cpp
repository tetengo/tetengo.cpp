/*! \file
    \brief A property set.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <memory>
#include <stdexcept>
#include <utility>

#include <boost/preprocessor.hpp>
#include <boost/scope_exit.hpp>

#include <tetengo/property/propertySet.h>
#include <tetengo/property/property_set.hpp>
#include <tetengo/property/storage.h>

#include "tetengo_property_storage.hpp"


struct tetengo_property_propertySet_tag
{
    std::unique_ptr<tetengo::property::property_set> p_cpp_property_set;

    explicit tetengo_property_propertySet_tag(std::unique_ptr<tetengo::property::property_set>&& p_cpp_property_set) :
    p_cpp_property_set{ std::move(p_cpp_property_set) }
    {}
};


tetengo_property_propertySet_t*
tetengo_property_propertySet_create(tetengo_property_storageLoader_t* const p_storage_loader, const char* const path)
{
    try
    {
        if (!p_storage_loader)
        {
            throw std::invalid_argument{ "p_storage_loader is NULL." };
        }
        BOOST_SCOPE_EXIT(p_storage_loader)
        {
            tetengo_property_storageLoader_destroy(p_storage_loader);
        }
        BOOST_SCOPE_EXIT_END;
        if (!path)
        {
            throw std::invalid_argument{ "path is NULL." };
        }

        auto p_cpp_storage_loader = std::move(p_storage_loader->p_cpp_storage_loader);
        auto p_cpp_property_set =
            std::make_unique<tetengo::property::property_set>(std::move(p_cpp_storage_loader), path);

        auto p_instance = std::make_unique<tetengo_property_propertySet_t>(std::move(p_cpp_property_set));
        return p_instance.release();
    }
    catch (...)
    {
        return nullptr;
    }
}

void tetengo_property_propertySet_destroy(const tetengo_property_propertySet_t* const p_property_set)
{
    try
    {
        const std::unique_ptr<const tetengo_property_propertySet_t> p_instance{ p_property_set };
    }
    catch (...)
    {}
}
