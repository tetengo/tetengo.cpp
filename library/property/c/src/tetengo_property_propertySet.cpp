/*! \file
    \brief A property set.

    Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/
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

bool tetengo_property_propertySet_getBool(
    const tetengo_property_propertySet_t* const p_property_set,
    const char* const                           key,
    bool* const                                 p_value)
{
    try
    {
        if (!p_property_set)
        {
            throw std::invalid_argument{ "p_property_set is NULL." };
        }
        if (!key)
        {
            throw std::invalid_argument{ "key is NULL." };
        }
        if (!p_value)
        {
            throw std::invalid_argument{ "p_value is NULL." };
        }

        const auto o_cpp_value = p_property_set->p_cpp_property_set->get_bool(key);
        if (o_cpp_value)
        {
            *p_value = *o_cpp_value;
        }
        return static_cast<bool>(o_cpp_value);
    }
    catch (...)
    {
        return false;
    }
}

void tetengo_property_propertySet_setBool(
    tetengo_property_propertySet_t* const p_property_set,
    const char* const                     key,
    const bool                            value)
{
    try
    {
        if (!p_property_set)
        {
            throw std::invalid_argument{ "p_property_set is NULL." };
        }
        if (!key)
        {
            throw std::invalid_argument{ "key is NULL." };
        }

        p_property_set->p_cpp_property_set->set_bool(key, value);
    }
    catch (...)
    {}
}

bool tetengo_property_propertySet_getUint32(
    const tetengo_property_propertySet_t* const p_property_set,
    const char* const                           key,
    uint32_t* const                             p_value)
{
    try
    {
        if (!p_property_set)
        {
            throw std::invalid_argument{ "p_property_set is NULL." };
        }
        if (!key)
        {
            throw std::invalid_argument{ "key is NULL." };
        }
        if (!p_value)
        {
            throw std::invalid_argument{ "p_value is NULL." };
        }

        const auto o_cpp_value = p_property_set->p_cpp_property_set->get_uint32(key);
        if (o_cpp_value)
        {
            *p_value = *o_cpp_value;
        }
        return static_cast<bool>(o_cpp_value);
    }
    catch (...)
    {
        return false;
    }
}

void tetengo_property_propertySet_setUint32(
    tetengo_property_propertySet_t* const p_property_set,
    const char* const                     key,
    const uint32_t                        value)
{
    try
    {
        if (!p_property_set)
        {
            throw std::invalid_argument{ "p_property_set is NULL." };
        }
        if (!key)
        {
            throw std::invalid_argument{ "key is NULL." };
        }

        p_property_set->p_cpp_property_set->set_uint32(key, value);
    }
    catch (...)
    {}
}

size_t tetengo_property_propertySet_getString(
    const tetengo_property_propertySet_t* const p_property_set,
    const char* const                           key,
    char* const                                 p_value,
    const size_t                                value_capacity)
{
    try
    {
        if (!p_property_set)
        {
            throw std::invalid_argument{ "p_property_set is NULL." };
        }
        if (!key)
        {
            throw std::invalid_argument{ "key is NULL." };
        }

        const auto o_cpp_value = p_property_set->p_cpp_property_set->get_string(key);
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
            return static_cast<size_t>(-1);
        }
    }
    catch (...)
    {
        return static_cast<size_t>(-1);
    }
}

void tetengo_property_propertySet_setString(
    tetengo_property_propertySet_t* const p_property_set,
    const char* const                     key,
    const char* const                     value)
{
    try
    {
        if (!p_property_set)
        {
            throw std::invalid_argument{ "p_property_set is NULL." };
        }
        if (!key)
        {
            throw std::invalid_argument{ "key is NULL." };
        }
        if (!value)
        {
            throw std::invalid_argument{ "value is NULL." };
        }

        p_property_set->p_cpp_property_set->set_string(key, value);
    }
    catch (...)
    {}
}

void tetengo_property_propertySet_update(tetengo_property_propertySet_t* const p_property_set)
{
    try
    {
        if (!p_property_set)
        {
            throw std::invalid_argument{ "p_property_set is NULL." };
        }

        p_property_set->p_cpp_property_set->update();
    }
    catch (...)
    {}
}

void tetengo_property_propertySet_commit(const tetengo_property_propertySet_t* const p_property_set)
{
    try
    {
        if (!p_property_set)
        {
            throw std::invalid_argument{ "p_property_set is NULL." };
        }

        p_property_set->p_cpp_property_set->commit();
    }
    catch (...)
    {}
}
