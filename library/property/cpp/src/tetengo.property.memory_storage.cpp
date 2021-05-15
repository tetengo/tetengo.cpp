/*! \file
    \brief A memory storage.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <filesystem>
#include <memory>

#include <boost/core/noncopyable.hpp>

#include <tetengo/property/memory_storage.hpp>
#include <tetengo/property/storage.hpp>


namespace tetengo::property
{
    class memory_storage::impl : private boost::noncopyable
    {
    public:
        // functions

        void save_impl() const {}
    };


    memory_storage::memory_storage() : m_p_impl{ std::make_unique<impl>() } {}

    memory_storage::~memory_storage() = default;

    void memory_storage::save_impl() const
    {
        m_p_impl->save_impl();
    }


    class memory_storage_loader::impl : private boost::noncopyable
    {
    public:
        // functions

        std::unique_ptr<storage> load_impl(const std::filesystem::path&) const
        {
            return std::make_unique<memory_storage>();
        }
    };


    memory_storage_loader::memory_storage_loader() : m_p_impl{ std::make_unique<impl>() } {}

    memory_storage_loader::~memory_storage_loader() = default;

    std::unique_ptr<storage> memory_storage_loader::load_impl(const std::filesystem::path& path) const
    {
        return m_p_impl->load_impl(path);
    }


}
