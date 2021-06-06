/*! \file
    \brief A property set.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <memory>
#include <utility>

#include <tetengo/property/propertySet.h>
#include <tetengo/property/property_set.hpp>


struct tetengo_property_propertySet_tag
{
    std::unique_ptr<tetengo::property::property_set> p_cpp_property_set;

    explicit tetengo_property_propertySet_tag(std::unique_ptr<tetengo::property::property_set>&& p_cpp_property_set) :
    p_cpp_property_set{ std::move(p_cpp_property_set) }
    {}
};


void tetengo_property_propertySet_destroy(const tetengo_property_propertySet_t* const p_property_set)
{
    try
    {
        const std::unique_ptr<const tetengo_property_propertySet_t> p_instance{ p_property_set };
    }
    catch (...)
    {}
}
