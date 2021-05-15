/*! \file
    \brief A file storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <filesystem>
#include <memory>
#include <utility>

#include <boost/core/noncopyable.hpp>

#include <tetengo/property/file_storage.hpp>
#include <tetengo/property/storage.hpp>


namespace tetengo::property
{
    class file_storage::impl : private boost::noncopyable
    {
    public:
        // types

        using value_map_type = file_storage::value_map_type;


        // functions

        void save_impl(const file_storage& /*self*/) const {}
    };


    file_storage::file_storage(value_map_type value_map) :
    storage{ std::move(value_map) },
        m_p_impl{ std::make_unique<impl>() }
    {}

    file_storage::~file_storage() = default;

    void file_storage::save_impl() const
    {
        m_p_impl->save_impl(*this);
    }


    class file_storage_loader::impl : private boost::noncopyable
    {
    public:
        // constructors

        impl() : m_master_value_map{} {}


        // functions

        std::unique_ptr<storage> load_impl(const std::filesystem::path&) const
        {
            return std::make_unique<file_storage>(storage::value_map_type{});
        }


    private:
        // variables

        mutable storage::value_map_type m_master_value_map;
    };


    file_storage_loader::file_storage_loader() : m_p_impl{ std::make_unique<impl>() } {}

    file_storage_loader::~file_storage_loader() = default;

    std::unique_ptr<storage> file_storage_loader::load_impl(const std::filesystem::path& path) const
    {
        return m_p_impl->load_impl(path);
    }


}
