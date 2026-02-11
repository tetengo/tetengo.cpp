/*! \file
    \brief A property set.

    Copyright (C) 2019-2026 kaoru  https://www.tetengo.org/
*/

#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
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
        m_p_storage{ m_p_storage_loader->load(m_path) },
        m_dirty{ false }
        {}


        // functions

        std::optional<bool> get_bool(const std::filesystem::path& key) const
        {
            return m_p_storage->get_bool(key);
        }

        void set_bool(const std::filesystem::path& key, const bool value)
        {
            m_p_storage->set_bool(key, value);
            m_dirty = true;
        }

        std::optional<std::uint32_t> get_uint32(const std::filesystem::path& key) const
        {
            return m_p_storage->get_uint32(key);
        }

        void set_uint32(const std::filesystem::path& key, const std::uint32_t value)
        {
            m_p_storage->set_uint32(key, value);
            m_dirty = true;
        }

        std::optional<std::string> get_string(const std::filesystem::path& key) const
        {
            return m_p_storage->get_string(key);
        }

        void set_string(const std::filesystem::path& key, const std::string& value)
        {
            m_p_storage->set_string(key, value);
            m_dirty = true;
        }

        void update()
        {
            m_p_storage = m_p_storage_loader->load(m_path);
            m_dirty = false;
        }

        void commit() const
        {
            if (!m_dirty)
            {
                return;
            }

            m_p_storage->save();
            m_dirty = false;
        }


    private:
        // variables

        const std::unique_ptr<storage_loader> m_p_storage_loader;

        const std::filesystem::path m_path;

        std::unique_ptr<storage> m_p_storage;

        mutable bool m_dirty;
    };


    property_set::property_set(std::unique_ptr<storage_loader>&& p_storage_loader, const std::filesystem::path& path) :
    m_p_impl{ std::make_unique<impl>(std::move(p_storage_loader), path) }
    {}

    property_set::~property_set() = default;

    std::optional<bool> property_set::get_bool(const std::filesystem::path& key) const
    {
        return m_p_impl->get_bool(key);
    }

    void property_set::set_bool(const std::filesystem::path& key, const bool value)
    {
        m_p_impl->set_bool(key, value);
    }

    std::optional<std::uint32_t> property_set::get_uint32(const std::filesystem::path& key) const
    {
        return m_p_impl->get_uint32(key);
    }

    void property_set::set_uint32(const std::filesystem::path& key, const std::uint32_t value)
    {
        m_p_impl->set_uint32(key, value);
    }

    std::optional<std::string> property_set::get_string(const std::filesystem::path& key) const
    {
        return m_p_impl->get_string(key);
    }

    void property_set::set_string(const std::filesystem::path& key, const std::string& value)
    {
        m_p_impl->set_string(key, value);
    }

    void property_set::update()
    {
        m_p_impl->update();
    }

    void property_set::commit() const
    {
        m_p_impl->commit();
    }


}
