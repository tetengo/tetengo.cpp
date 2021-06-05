/*! \file
    \brief A storage proxy.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>

#include <boost/core/noncopyable.hpp>

#include <tetengo/property/storage.hpp>
#include <tetengo/property/storage_proxy.hpp>


namespace tetengo::property
{
    class storage_proxy::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        impl(std::shared_ptr<storage_loader> p_real_storage_loader, const std::filesystem::path& path) :
        m_p_real_storage_loader{ std::move(p_real_storage_loader) },
            m_path{ path }
        {}


        // functions

        std::optional<bool> get_bool_impl(const std::filesystem::path& key) const
        {
            ensure_real_loaded();
            return m_p_real_storage->get_bool(key);
        }

        void set_bool_impl(const std::filesystem::path& key, const bool value)
        {
            ensure_real_loaded();
            m_p_real_storage->set_bool(key, value);
        }

        std::optional<std::uint32_t> get_uint32_impl(const std::filesystem::path& key) const
        {
            ensure_real_loaded();
            return m_p_real_storage->get_uint32(key);
        }

        void set_uint32_impl(const std::filesystem::path& key, const std::uint32_t value)
        {
            ensure_real_loaded();
            m_p_real_storage->set_uint32(key, value);
        }

        std::optional<std::string> get_string_impl(const std::filesystem::path& key) const
        {
            ensure_real_loaded();
            return m_p_real_storage->get_string(key);
        }

        void set_string_impl(const std::filesystem::path& key, const std::string& value)
        {
            ensure_real_loaded();
            m_p_real_storage->set_string(key, value);
        }

        void save_impl() const
        {
            if (!m_p_real_storage)
            {
                return;
            }
            m_p_real_storage->save();
        }


    private:
        // variables

        const std::shared_ptr<storage_loader> m_p_real_storage_loader;

        const std::filesystem::path m_path;

        mutable std::unique_ptr<storage> m_p_real_storage;


        // functions

        void ensure_real_loaded() const
        {
            if (m_p_real_storage)
            {
                return;
            }
            m_p_real_storage = m_p_real_storage_loader->load(m_path);
        }
    };


    storage_proxy::~storage_proxy() = default;

    storage_proxy::storage_proxy(
        std::shared_ptr<storage_loader> p_real_storage_loader,
        const std::filesystem::path&    path) :
    storage{ value_map_type{} },
        m_p_impl{ std::make_unique<impl>(std::move(p_real_storage_loader), path) }
    {}

    std::optional<bool> storage_proxy::get_bool_impl(const std::filesystem::path& key) const
    {
        return m_p_impl->get_bool_impl(key);
    }

    void storage_proxy::set_bool_impl(const std::filesystem::path& key, const bool value)
    {
        m_p_impl->set_bool_impl(key, value);
    }

    std::optional<std::uint32_t> storage_proxy::get_uint32_impl(const std::filesystem::path& key) const
    {
        return m_p_impl->get_uint32_impl(key);
    }

    void storage_proxy::set_uint32_impl(const std::filesystem::path& key, const std::uint32_t value)
    {
        m_p_impl->set_uint32_impl(key, value);
    }

    std::optional<std::string> storage_proxy::get_string_impl(const std::filesystem::path& key) const
    {
        return m_p_impl->get_string_impl(key);
    }

    void storage_proxy::set_string_impl(const std::filesystem::path& key, const std::string& value)
    {
        m_p_impl->set_string_impl(key, value);
    }

    void storage_proxy::save_impl() const
    {
        m_p_impl->save_impl();
    }


    class storage_loader_proxy::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        explicit impl(std::unique_ptr<storage_loader>&& p_real_storage_loader) :
        m_p_real_storage_loader{ std::move(p_real_storage_loader) }
        {
            if (!m_p_real_storage_loader)
            {
                throw std::invalid_argument{ "p_real_storage_loader is nullptr." };
            }
        }


        // functions

        std::unique_ptr<storage> load_impl(const std::filesystem::path& path) const
        {
            struct storage_impl : public storage_proxy
            {
                storage_impl(std::shared_ptr<storage_loader> p_real_storage_loader, const std::filesystem::path& path) :
                storage_proxy{ std::move(p_real_storage_loader), path }
                {}
            };
            return std::make_unique<storage_impl>(m_p_real_storage_loader, path);
        }


    private:
        // variables

        const std::shared_ptr<storage_loader> m_p_real_storage_loader;
    };


    storage_loader_proxy::storage_loader_proxy(std::unique_ptr<storage_loader>&& p_real_storage_loader) :
    m_p_impl{ std::make_unique<impl>(std::move(p_real_storage_loader)) }
    {}

    storage_loader_proxy::~storage_loader_proxy() = default;

    std::unique_ptr<storage> storage_loader_proxy::load_impl(const std::filesystem::path& path) const
    {
        return m_p_impl->load_impl(path);
    }


}
