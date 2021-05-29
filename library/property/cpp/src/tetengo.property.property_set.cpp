/*! \file
    \brief A property set.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <filesystem>
#include <memory>
#include <utility>

#include <boost/core/noncopyable.hpp>

#include <tetengo/property/property_set.hpp>
#include <tetengo/property/storage.hpp>


namespace tetengo::property
{
    class property_set::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        impl(std::unique_ptr<storage_loader>&& p_storage_loader, const std::filesystem::path& path) :
        m_p_storage_loader{ std::move(p_storage_loader) },
            m_path{ path },
            m_p_storage{ m_p_storage_loader->load(m_path) }
        {}


        // functions

        void update()
        {
            m_p_storage = m_p_storage_loader->load(m_path);
        }

        void commit() const
        {
            m_p_storage->save();
        }


    private:
        // variables

        const std::unique_ptr<storage_loader> m_p_storage_loader;

        const std::filesystem::path m_path;

        std::unique_ptr<storage> m_p_storage;
    };


    property_set::property_set(std::unique_ptr<storage_loader>&& p_storage_loader, const std::filesystem::path& path) :
    m_p_impl{ std::make_unique<impl>(std::move(p_storage_loader), path) }
    {}

    property_set::~property_set() = default;

    void property_set::update()
    {
        m_p_impl->update();
    }

    void property_set::commit() const
    {
        m_p_impl->commit();
    }


}
