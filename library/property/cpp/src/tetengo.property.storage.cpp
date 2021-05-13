/*! \file
    \brief A storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <memory>

#include <tetengo/property/storage.hpp>


namespace tetengo::property
{
    storage::~storage() = default;


    storage_factory::~storage_factory() = default;

    std::unique_ptr<storage> storage_factory::load() const
    {
        return load_impl();
    }


}
