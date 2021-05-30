/*! \file
    \brief A storage proxy.

    Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/
*/

#include <filesystem>
#include <memory>
#include <utility>

#include <boost/core/noncopyable.hpp>

#include <tetengo/property/storage.hpp>
#include <tetengo/property/storage_proxy.hpp>


namespace tetengo::property
{
    class storage_proxy::impl : private boost::noncopyable
    {
    public:
        // types

        using value_map_type = storage_proxy::value_map_type;


        // constructors and destructor

        impl(std::shared_ptr<storage_loader> p_real_storage_loader, const std::filesystem::path& path) :
        m_p_readl_storage_loader{ std::move(p_real_storage_loader) },
            m_path{ path }
        {}


        // functions

        void save_impl(const storage_proxy& /*self*/) const {}


    private:
        // variables

        const std::shared_ptr<storage_loader> m_p_readl_storage_loader;

        const std::filesystem::path m_path;
    };


    storage_proxy::storage_proxy(
        std::shared_ptr<storage_loader> p_real_storage_loader,
        const std::filesystem::path&    path) :
    storage{ value_map_type{} },
        m_p_impl{ std::make_unique<impl>(std::move(p_real_storage_loader), path) }
    {}

    storage_proxy::~storage_proxy() = default;

    void storage_proxy::save_impl() const
    {
        m_p_impl->save_impl(*this);
    }


    class storage_proxy_loader::impl : private boost::noncopyable
    {
    public:
        // constructors and destructor

        explicit impl(std::unique_ptr<storage_loader>&& p_real_storage_loader) :
        m_p_real_storage_loader{ std::move(p_real_storage_loader) }
        {}


        // functions

        std::unique_ptr<storage> load_impl(const std::filesystem::path& path) const
        {
            return std::make_unique<storage_proxy>(m_p_real_storage_loader, path);
        }


    private:
        // variables

        const std::shared_ptr<storage_loader> m_p_real_storage_loader;
    };


    storage_proxy_loader::storage_proxy_loader(std::unique_ptr<storage_loader>&& p_real_storage_loader) :
    m_p_impl{ std::make_unique<impl>(std::move(p_real_storage_loader)) }
    {}

    storage_proxy_loader::~storage_proxy_loader() = default;

    std::unique_ptr<storage> storage_proxy_loader::load_impl(const std::filesystem::path& path) const
    {
        return m_p_impl->load_impl(path);
    }


}
