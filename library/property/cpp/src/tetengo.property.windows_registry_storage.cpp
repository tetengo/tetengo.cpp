/*! \file
    \brief A Windows registry storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <filesystem>
#include <memory>

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


        // constructors and destructor

        impl() {}


        // functions

        void save_impl(const windows_registry_storage& /*self*/) const {}


    private:
        // variables
    };


    windows_registry_storage::windows_registry_storage() :
    storage{ value_map_type{} },
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
        // constructors

        impl() {}


        // functions

        std::unique_ptr<storage> load_impl(const std::filesystem::path&) const
        {
            return std::make_unique<windows_registry_storage>();
        }
    };


    windows_registry_storage_loader::windows_registry_storage_loader() : m_p_impl{ std::make_unique<impl>() } {}

    windows_registry_storage_loader::~windows_registry_storage_loader() = default;

    std::unique_ptr<storage> windows_registry_storage_loader::load_impl(const std::filesystem::path& path) const
    {
        return m_p_impl->load_impl(path);
    }


}
