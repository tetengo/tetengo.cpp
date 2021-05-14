/*! \file
    \brief A storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>

#include <tetengo/property/storage.hpp>


namespace tetengo::property
{
    storage::~storage() = default;

    std::optional<std::uint32_t> storage::get_uint32(const std::filesystem::path& key) const
    {
        return get_uint32_impl(key);
    }

    void storage::set_uint32(const std::filesystem::path& key, const std::uint32_t value)
    {
        set_uint32_impl(key, value);
    }

    void storage::save() const
    {
        return save_impl();
    }


    storage_loader::~storage_loader() = default;

    std::unique_ptr<storage> storage_loader::load(const std::filesystem::path& path) const
    {
        return load_impl(path);
    }


}
