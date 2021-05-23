/*! \file
    \brief A Windows registry storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <filesystem>
#include <memory>
#include <utility>

#include <boost/core/noncopyable.hpp>

#include <tetengo/property/storage.hpp>
#include <tetengo/property/windows_registry_storage.hpp>


namespace tetengo::property
{
    class windows_registry_storage::impl : private boost::noncopyable
    {
    public:
        // types

        using value_map_type = windows_registry_storage::value_map_type;


        // functions

        void save_impl(const windows_registry_storage& /*self*/) const {}


    private:
        // variables
    };


    windows_registry_storage::windows_registry_storage(value_map_type value_map) :
    storage{ std::move(value_map) },
        m_p_impl{ std::make_unique<impl>() }
    {}

    windows_registry_storage::~windows_registry_storage() = default;

    void windows_registry_storage::save_impl() const
    {
        m_p_impl->save_impl(*this);
    }


    class windows_registry_storage_loader::impl : private boost::noncopyable
    {
    public:
        // functions

        std::unique_ptr<storage> load_impl(const std::filesystem::path& /*path*/) const
        {
            return std::make_unique<windows_registry_storage>(value_map_type{});
        }


    private:
        // types

        using value_map_type = storage::value_map_type;
    };


    windows_registry_storage_loader::windows_registry_storage_loader() : m_p_impl{ std::make_unique<impl>() } {}

    windows_registry_storage_loader::~windows_registry_storage_loader() = default;

    std::unique_ptr<storage> windows_registry_storage_loader::load_impl(const std::filesystem::path& path) const
    {
        return m_p_impl->load_impl(path);
    }


}
