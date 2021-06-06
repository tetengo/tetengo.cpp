/*! \file
    \brief A storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#if !defined(TETENGO_PROPERTY_STORAGE_HPP_)
#define TETENGO_PROPERTY_STORAGE_HPP_


#include <memory>
#include <utility>

#include <tetengo/property/storage.hpp>


struct tetengo_property_storageLoader_tag
{
    std::unique_ptr<tetengo::property::storage_loader> p_cpp_storage_loader;

    explicit tetengo_property_storageLoader_tag(
        std::unique_ptr<tetengo::property::storage_loader>&& p_cpp_storage_loader) :
    p_cpp_storage_loader{ std::move(p_cpp_storage_loader) }
    {}
};


#endif
